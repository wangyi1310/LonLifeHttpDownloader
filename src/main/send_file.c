#include "send_file.h"

void add_header(char *response, Map *pmap) {
	strcat(response, (const char *)value(pmap, "protype"));
	strcat(response, " ");
	strcat(response, (const char *)value(pmap, "status"));
	strcat(response, " ");
	strcat(response, (const char *)value(pmap, "mean"));
	strcat(response, "\r\n");


	strcat(response, "Server: ");
	strcat(response, (const char *)value(pmap, "Server"));
	strcat(response, "\r\n");


	strcat(response, "Content-Length: ");
	strcat(response, (const char *)value(pmap, "Content-Length"));
	strcat(response, "\r\n");

	strcat(response, "\r\n");
}


void get_header_msg(Map *pmap, int status, const char *mean, int length) {
	add_item(pmap, new_item("protype", "HTTP/1.1", 9));
	char buf[10] = "";
	sprintf(buf, "%d", status);
	add_item(pmap, new_item("status", buf, strlen(buf)));
	add_item(pmap, new_item("Server", "Simple Http Server", 19));
	add_item(pmap, new_item("mean", (void *)mean, strlen(mean)));
	memset(buf, 0, strlen(buf));
	sprintf(buf, "%d", length);
	add_item(pmap, new_item("Content-Length", buf, strlen(buf)));
	// if you want to add it just do like this
}

int get_file_size(const char *file_name) {
	struct stat statbuf;
	if (stat(file_name, &statbuf) < 0) {
		perror("STAT FILE");
		return 0;
	}
	else {
		return statbuf.st_size;
	}
}

void write_to_client(int fd, const char *file_name) {
	Map header_map = map();
	char *response = (char *)calloc(sizeof(char), 128);
	int total_len = get_file_size(file_name);
	get_header_msg(&header_map, 200, "OK", total_len);
	add_header(response, &header_map);

	// main message
	send (fd, response, strlen(response)+1, 0);
	//open file
	int filefd = open(file_name, O_RDONLY);
	assert (filefd != -1);
	int len = 0;
	while (len < total_len) {
		memset(response, 0, strlen(response));
		len += read(filefd, response, 127); 
		send(fd, response, strlen(response)+1, 0); 
	}
	close(filefd);
	free(response);
	map_clear(&header_map);
}