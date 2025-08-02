/* SPDX-License-Identifier: ISC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

const char *d_types[] = {
	[DT_BLK] = "block",
	[DT_CHR] = "char",
	[DT_DIR] = "dir",
	[DT_FIFO] = "fifo",
	[DT_LNK] = "symlink",
	[DT_REG] = "file",
	[DT_SOCK] = "socket",
	[DT_UNKNOWN] = "???",
};

[[noreturn]] void usage(void)
{
	fprintf(stderr, "usage: treescan dir > dir.list\n");
	exit(EXIT_FAILURE);
}

bool scan(const char *root)
{
	DIR *dir;
	bool result = true;

	dir = opendir(root);
	if (!dir)
		return false;

	struct dirent *dentry;
	for (dentry = readdir(dir); dentry != NULL; dentry = readdir(dir))
	{
		char targetpath[16384];

		if (dentry->d_name[0] == '.' && (dentry->d_name[1] == '.' || !dentry->d_name[1]))
			continue;

		snprintf(targetpath, sizeof targetpath, "%s/%s", root, dentry->d_name);

		struct stat st;
		if (stat(targetpath, &st) == -1)
		{
			result = false;
			goto out;
		}

		printf("%s type=%s mtime=%zd\n", targetpath, d_types[dentry->d_type], st.st_mtime);

		if (dentry->d_type == DT_DIR)
		{
			if (!scan(targetpath))
			{
				result = false;
				goto out;
			}

			continue;
		}
	}	

out:
	closedir(dir);
	return result;
}

int main(int argc, const char **argv)
{
	if (argc < 2)
		usage();

	if (!scan(argv[1]))
	{
		fprintf(stderr, "scanning %s: %s\n", argv[1], strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
