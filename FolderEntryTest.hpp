#include "DetailBFS.hpp"
#include "DetailFileBlock.hpp"
#include "FileEntry.hpp"
#include "FolderEntry.hpp"
#include "MakeBFS.hpp"
#include "TestHelpers.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/stream.hpp>

#include <cassert>
#include <sstream>
#include <fstream>


class FolderEntryTest
{


  public:
	FolderEntryTest() : m_uniquePath(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path())
    {
        boost::filesystem::create_directories(m_uniquePath);
        testAddEntry();
    }

    ~FolderEntryTest()
    {
        boost::filesystem::remove_all(m_uniquePath);
    }

  private:

    boost::filesystem::path m_uniquePath;

    void testAddEntry()
    {
        std::string testImage(boost::filesystem::unique_path().string());
        boost::filesystem::path testPath = m_uniquePath / testImage;
        uint64_t blocks(2048); // 1MB

        {
			  bfs::MakeBFS bfs(testPath.string(), blocks);
        }

        //
        // when entry 0 added to root folder, root folder block starts at 0
        // this creates a new file entry at block 1
        // when entry 1 added, still at block 0, this creates a new file entry
        // at block 2; folder entry ends up also writing to block 3
        // when entry 2 added, root folder at block 3, this creates a new file
        // entry at block 4
        // when entry 3 added, root folder still at block 3, this creates a new
        // file entry at block 5. Root folder ends up also writing to block 6
        //

        {
			bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
			folder.addFileEntry("test.txt");
			folder.addFileEntry("fucker.log");
			folder.addFileEntry("crap.jpg");
			folder.addFileEntry("shitter.mp3");
			ASSERT_EQUAL(folder.getEntryName(0), "test.txt", "Folder entry get entry test");
            ASSERT_EQUAL(folder.getEntryName(1), "fucker.log", "Folder entry get entry test");
            ASSERT_EQUAL(folder.getEntryName(2), "crap.jpg", "Folder entry get entry test");
            ASSERT_EQUAL(folder.getEntryName(3), "shitter.mp3", "Folder entry get entry test");
        }

        std::cout<<"Test adding folder entries (files) plus name retrieval passed.."<<std::endl;


        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry fa = folder.getFileEntry("test.txt");
            bfs::FileEntry fb = folder.getFileEntry("fucker.log");
            bfs::FileEntry fc = folder.getFileEntry("crap.jpg");
            bfs::FileEntry fd = folder.getFileEntry("shitter.mp3");
            uint64_t b1 = fa.getStartBlockIndex();
            uint64_t b2 = fb.getStartBlockIndex();
            uint64_t b3 = fc.getStartBlockIndex();
            uint64_t b4 = fd.getStartBlockIndex();

            // see comment above for expectations
            ASSERT_EQUAL(b1, 1, "Folder entry get start block index")
            ASSERT_EQUAL(b2, 2, "Folder entry get start block index")
            ASSERT_EQUAL(b3, 4, "Folder entry get start block index")
            ASSERT_EQUAL(b4, 5, "Folder entry get start block index")
        }

        std::cout<<"Test file entry block index retrieval passed.."<<std::endl;

        std::string testData("some test data!");
        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry = folder.getFileEntry("fucker.log");
            std::vector<uint8_t> vec(testData.begin(), testData.end());
            entry.write((char*)&vec.front(), testData.length());
            entry.flush();
        }

        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry(folder.getFileEntry("fucker.log"));
            entry.seek(0); // seek to start since retrieving from folder automatically seeks to end
            std::vector<uint8_t> vec;
            vec.resize(entry.fileSize());
            entry.read((char*)&vec.front(), entry.fileSize());
            std::string result(vec.begin(), vec.end());
            ASSERT_EQUAL(result, testData, "Folder entry written data test")
        }

        std::cout<<"Test file entry retrieval plus appending of small data passed.."<<std::endl;
        std::string testString(createLargeStringToWrite());
        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry = folder.getFileEntry("fucker.log");
            std::vector<uint8_t> vec(testString.begin(), testString.end());
            entry.write((char*)&vec.front(), testString.length());
            entry.flush();
        }

        std::string origTestData = testData;

        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry(folder.getFileEntry("fucker.log"));
            entry.seek(0); // seek to start since retrieving from folder automatically seeks to end
            std::vector<uint8_t> vec;
            vec.resize(entry.fileSize());
            entry.read((char*)&vec.front(), entry.fileSize());
            std::string result(vec.begin(), vec.end());
            assert(result == (testData.append(testString)));
        }

        testData = origTestData;

        std::cout<<"Test file entry retrieval plus appending of large data to already appended small data passed.."<<std::endl;

        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry = folder.getFileEntry("crap.jpg");
            std::vector<uint8_t> vec(testString.begin(), testString.end());
            entry.write((char*)&vec.front(), testString.length());
            entry.flush();
        }

        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry(folder.getFileEntry("crap.jpg"));
            entry.seek(0); // seek to start since retrieving from folder automatically seeks to end
            std::vector<uint8_t> vec;
            vec.resize(entry.fileSize());
            entry.read((char*)&vec.front(), entry.fileSize());
            std::string result(vec.begin(), vec.end());
            assert(result == testString);
        }

        std::cout<<"Test another file entry retrieval plus appending of large data passed.."<<std::endl;

        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry = folder.getFileEntry("crap.jpg");
            entry.seek(0, std::ios::end);
            std::vector<uint8_t> vec(testData.begin(), testData.end());
            entry.write((char*)&vec.front(), testData.length());
            entry.flush();
        }


        {
            bfs::FolderEntry folder(testPath.string(), blocks, 0, "root");
            bfs::FileEntry entry(folder.getFileEntry("crap.jpg"));
            entry.seek(0); // seek to start since retrieving from folder automatically seeks to end
            std::vector<uint8_t> vec;
            vec.resize(entry.fileSize());
            entry.read((char*)&vec.front(), entry.fileSize());
            std::string result(vec.begin(), vec.end());
            assert(result == (testString.append(testData)));
        }

        std::cout<<"Test another file entry retrieval plus appending of small data to already appended large data passed.."<<std::endl;

    }
};