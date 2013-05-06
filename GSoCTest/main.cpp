/* GSoC programming test
*/

#include "../triplegit/include/async_file_io.hpp"
#include <iostream>

#include "boost/detail/lightweight_test.hpp"


std::pair<std::vector<triplegit::async_io::future<std::vector<std::filesystem::path>>>, std::vector<triplegit::async_io::async_io_op>> triplegit::async_io::async_file_io_dispatcher_base::
	enumerate(const std::vector<async_io_op> &ops, const std::vector<std::filesystem::path> &paths)
{
	assert(ops.size() == paths.size());
	auto doEnum = [](std::filesystem::path p){
		std::vector<std::filesystem::path> vec;
		vec.push_back(p);
		//for( auto it = std::filesystem::directory_iterator(p); it != std::filesystem::directory_iterator(); ++it)
			//vec.push_back((*it).path().filename());
		return vec;
	};
	std::vector<std::function<std::vector<std::filesystem::path>()> > callbacks;
	callbacks.reserve(ops.size());
	for(auto i = 0; i < paths.size(); ++i)
		callbacks.push_back(std::bind(doEnum, paths[i]));		

	return call(ops, callbacks);
}




int main(int argc, const char *argv[])
{
	using namespace triplegit::async_io;
	using namespace std;
	using triplegit::async_io::future;
	
	auto dispatcher=triplegit::async_io::async_file_io_dispatcher(triplegit::async_io::process_threadpool(), triplegit::async_io::file_flags::None);
	auto mkdir(dispatcher->dir(async_path_op_req("testdir", file_flags::Create)));


	//create 100 sub directories to testdir
	std::vector<triplegit::async_io::async_path_op_req> many_dir_reqs;
	many_dir_reqs.reserve(100);
	for(size_t n=0; n<100; n++)
		many_dir_reqs.push_back(async_path_op_req(mkdir, "testdir/"+std::to_string(n), file_flags::Create));
	auto many_dirs(dispatcher->dir(many_dir_reqs));

	// create 10 files per folder
	std::vector<triplegit::async_io::async_path_op_req> manyfilereqs;
	manyfilereqs.reserve(1000);
	for(size_t n = 0; n < 100; ++n)
	{
		auto precondition = many_dirs[n];
		for(size_t m=0; m < 10; ++m)
			manyfilereqs.push_back(async_path_op_req(precondition,"testdir/"+std::to_string(n) + "/" + std::to_string(m), 
			file_flags::Create|file_flags::Write));
	}
	auto manyopenfiles(dispatcher->file(manyfilereqs));// create files

	// Close each of those 1000 files as they are opened
	auto manyclosedfiles(dispatcher->close(manyopenfiles));


	
	
	when_all(manyclosedfiles.begin(), manyclosedfiles.end()).wait();//when all files are closed

	
	
	/*for(auto p : paths)
	{
		//cout << p.id << endl;
		//cout << p << endl;
		for( auto it = std::filesystem::directory_iterator(p); it != std::filesystem::directory_iterator(); ++it)
			cout << (*it).path() <<endl;
	}//*/


	/*// set preconditions
	std::vector<triplegit::async_io::async_io_op> enum_op; // preconditions for enumerate
	enum_op.reserve(paths.size());							//should be the same size as the number of paths we give it

	auto precondition(triplegit::async_io::async_io_op(manyclosedfiles.back()));	// the precondition is that all files be closed
	for(auto m: paths)
		enum_op.push_back(precondition);								//put the precondition in the vector 
	auto output = dispatcher->enumerate(enum_op, paths);//*/			// enumerate the paths



	// create vector of paths
	std::vector<std::filesystem::path> paths;
	paths.reserve(many_dir_reqs.size());
	for(auto &i : many_dir_reqs)
		paths.push_back(i.path);
	

	std::vector<std::filesystem::path> path_test;  
	//path_test.push_back(  async_path_op_req("testdir", file_flags::Create).path);
	//path_test.push_back(paths[27]);
	//path_test.push_back(paths[32]);
	//path_test.push_back(paths[87]);
	std::vector<triplegit::async_io::async_io_op> op_test;
	//op_test.push_back(many_dirs.back());
	//op_test.push_back(manyclosedfiles.back());
	//op_test.push_back(manyclosedfiles.back());
	//op_test.push_back(manyclosedfiles.back());
	for (auto i = 0; i < 30; ++i)
	{		path_test.push_back(paths[i]);

		op_test.push_back(manyclosedfiles.back());
	}
	auto output(dispatcher->enumerate(op_test, path_test));//*/

	

	//when_all(output.first.begin(), output.first.end()).wait();
	when_all(output.second.begin(), output.second.end()).wait();	// wait until enumerations are complete
	
	for(auto &fut : output.first)  //fut is a future holding a vector of paths
	{
		//fut.wait();
		for( auto vec_path : fut.get())
			cout << vec_path << endl;	
	}


	// Delete each of the files once they are closed

	/*for(auto op :manyclosedfiles)
		for(auto &i : manyfilereqs)
			i.precondition=op;*/
	auto it(manyclosedfiles.begin());
	for(auto &i : manyfilereqs)
		i.precondition=*it++;
	auto manydeletedfiles(dispatcher->rmfile(manyfilereqs));

	
	when_all(manydeletedfiles.begin(), manydeletedfiles.end()).wait();
	auto many_deleted_dirs(dispatcher->rmdir(many_dir_reqs));
	when_all(many_deleted_dirs.begin(), many_deleted_dirs.end()).wait();
	auto rmdir(dispatcher->rmdir(async_path_op_req("testdir")));//*/

	
	
	//for(auto output.first
	//cout << (*(*output.first.begin()).get().begin()) <<endl;//*/

	rmdir.h.get();
	system("pause");
    return 0;
}

/*


using namespace triplegit::async_io;
	using namespace std;
	using triplegit::async_io::future;
	
	auto dispatcher=triplegit::async_io::async_file_io_dispatcher(triplegit::async_io::process_threadpool(), triplegit::async_io::file_flags::None);
	auto mkdir(dispatcher->dir(async_path_op_req("testdir", file_flags::Create)));


	//create 100 sub directories to testdir
	std::vector<triplegit::async_io::async_path_op_req> many_dir_reqs;
	many_dir_reqs.reserve(100);
	for(size_t n=0; n<100; n++)
		many_dir_reqs.push_back(async_path_op_req(mkdir, "testdir/"+std::to_string(n), file_flags::Create));
	auto many_dirs(dispatcher->dir(many_dir_reqs));

	// create 10 files per folder
	std::vector<triplegit::async_io::async_path_op_req> manyfilereqs;
	manyfilereqs.reserve(1000);

	for(size_t n = 0; n < 100; ++n)
	{
		auto precondition = many_dirs[n];
		for(size_t m=0; m < 10; ++m)
			manyfilereqs.push_back(async_path_op_req(precondition,"testdir/"+std::to_string(n) + "/" + std::to_string(m), 
			file_flags::Create|file_flags::Write));
	}
	auto manyopenfiles(dispatcher->file(manyfilereqs));// create files

	// Close each of those 1000 files as they are opened
	auto manyclosedfiles(dispatcher->close(manyopenfiles));


	// create vector of paths
	std::vector<std::filesystem::path> paths;
	paths.reserve(many_dir_reqs.size());
	for(auto &i : many_dir_reqs)
		paths.push_back(i.path);
	
	
	//when_all(manyclosedfiles.begin(), manyclosedfiles.end()).wait();//when all files are closed


	/*for(auto p : paths)
	{
		//cout << p.id << endl;
		cout << p << endl;
	//	for( auto it = std::filesystem::directory_iterator(p); it != std::filesystem::directory_iterator(); ++it)
		//	cout << *it <<endl;
	}//*/

/*	// set preconditions
	std::vector<triplegit::async_io::async_io_op> enum_req;
	enum_req.reserve(paths.size());
	//auto precondition(triplegit::async_io::async_io_op(manyclosedfiles.back()));
	for(auto m: paths)
		enum_req.push_back(manyclosedfiles.back());
	auto output = dispatcher->enumerate(enum_req, paths);//*/
	
	/*
	std::vector<std::filesystem::path> path_test;  
	path_test.push_back(  async_path_op_req("testdir", file_flags::Create).path);
	
	std::vector<triplegit::async_io::async_io_op> op_test;
	op_test.push_back(many_dirs.back());
	for (auto i = 0; i < 1; ++i)
	{
		path_test.push_back(paths[i]);

		op_test.push_back(triplegit::async_io::async_io_op(manyclosedfiles.back()));
	}
	auto output = dispatcher->enumerate(op_test, path_test);//*/
	
	
/*
	//when_all(output.first.begin(), output.first.end()).wait();
	when_all(output.second.begin(), output.second.end()).wait();

	// Delete each of the files once they are closed

	/*for(auto op :output.second)
		for(auto &i : manyfilereqs)
			i.precondition=op;*//*
	auto manydeletedfiles(dispatcher->rmfile(manyfilereqs));

	
	when_all(manydeletedfiles.begin(), manydeletedfiles.end()).wait();
	auto many_deleted_dirs(dispatcher->rmdir(many_dir_reqs));
	when_all(many_deleted_dirs.begin(), many_deleted_dirs.end()).wait();
	auto rmdir(dispatcher->rmdir(async_path_op_req("testdir")));//*/

	
	/*for(auto &fut : output.first)  //fut is a future holding a vector of paths
	{
		//fut.wait();
		for( auto vec_path : fut.get())
			cout << vec_path << endl;	
	}
	//for(auto output.first
	//cout << (*(*output.first.begin()).get().begin()) <<endl;//*/

