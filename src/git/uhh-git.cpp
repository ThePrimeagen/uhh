#include "uhh-git.h"

#include <cstdio>
#include <sstream>
#include <stdlib.h>

namespace fs = std::filesystem;

int Git::commit(const UhhConfig& config, const std::string& repoPath) {

    // TODO: probably should not do this.  It somehow feels worse than below,
    // but then again, there is below, which is not pretty.  Justn use the cpp
    // wrapper at some point in the near future.
    //
    // Aka, I'll never do it and get flamed on the interwebs at somepoint.
    std::stringstream add;
    add << "cd " << repoPath << " && git add .";
    int err = system(add.str().c_str());
    if (err != 0) {
        return err;
    }

    std::stringstream commit;
    commit << "cd " << repoPath << " && git commit -m \"uhh auto commit.\"";
    err = system(commit.str().c_str());
    if (err != 0) {
        return err;
    }

    std::stringstream push;
    push << "cd " << repoPath << " && git push origin master";
    err = system(push.str().c_str());
    if (err != 0) {
        return err;
    }

    return 0;

    /*
    git_repository* repo;
    git_index *index;
	const git_index_entry *entry;
	git_oid tree_oid; //, expected_blob_oid, expected_tree_oid, commit_oid, expected_commit_oid;
	git_signature *signature;
	git_tree *tree;
	git_buf buffer;

    git_repository_init(&repo, repoPath.c_str(), 0);

	git_repository_index(&index, repo);
	git_index_add_bypath(index, ".");

	entry = git_index_get_byindex(index, 0);
	git_index_write_tree(&tree_oid, index);

    std::time_t result = std::time(nullptr);
    std::asctime(std::localtime(&result));

	git_signature_new(&signature, "uhh", "uhh@uhh.uhh", result, 60));
	cl_git_pass(git_tree_lookup(&tree, repo, &tree_oid));

	memset(&buffer, 0, sizeof(git_buf));
	cl_git_pass(git_message_prettify(&buffer, "Initial commit", 0, '#'));

	cl_git_pass(git_commit_create_v(
		&commit_oid,
		repo,
		"HEAD",
		signature,
		signature,
		NULL,
		buffer.ptr,
		tree,
		0));

	cl_assert(git_oid_cmp(&expected_commit_oid, &commit_oid) == 0);

	git_buf_free(&buffer);
	git_signature_free(signature);
	git_tree_free(tree);
	git_index_free(index);

    return 0;
    */
}

int Git::clone(const fs::path& configPath, const std::string& repoUrl) {
    /*
    git_repository *repo = NULL;
    const char *url = config.repoUrl.c_str();
    const char *path = repoPath.c_str();

    git_clone_options clone_opts = GIT_STATUS_OPTIONS_INIT;
    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
    fetch_opts.callbacks.credentials = credentials_cb;

    clone_opts.fetch_opts = fetch_opts;

    return git_clone(&repo, url, path, &clone_opts);
    */

    std::stringstream clone;
    clone << "cd " << configPath << " && git clone " << repoUrl << " repo";
    return system(clone.str().c_str());
}

void Git::printError(int error) {
    const git_error *e = git_error_last();
    if (e == NULL) {

        printf("Error: You do not use my library correctly.  This makes me mad.  I am going to write a very long message about how I feel about this.\n");
        printf("When I was a child, i would run and play with my cat.  The cat was very nice.  But there were times that I was too aggressive and that caused the cat to bite me.  It was my fault.  Much like this moment in time, its likely your fault.  Don't do this to you or anyone else.  Its very rude.\n");

        // double F u
        exit(1 + 3 + 3 + 7);
    }

    printf("Error %d/%d: %s\n", error, e->klass, e->message);
}

