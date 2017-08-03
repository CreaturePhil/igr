#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_URL_LEN 48
#define BUF_LEN     128

int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
  FILE *tmpFile;
  FILE *destFile;
  char url[MAX_URL_LEN];
  char *base_url = "https://www.gitignore.io/api/";
  int len = strlen(base_url);

  if (argc != 2) {
    printf("Must have 1 argument: igr [file]\n");
    return 0;
  }

  remove(".gitignore");

  url[0] = '\0';
  strncat(url, base_url, len+1);
  strncat(url, argv[1], MAX_URL_LEN-len);
  url[MAX_URL_LEN-1] = '\0';

  curl = curl_easy_init();

  if (curl) {
    tmpFile = fopen("temp.gitignore", "w");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmpFile);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      fclose(tmpFile);
      curl_easy_cleanup(curl);
      return 0;
    }

    fclose(tmpFile);
    curl_easy_cleanup(curl);

    tmpFile = fopen("temp.gitignore", "r");
    destFile = fopen(".gitignore", "a");

    char buf[BUF_LEN];
    int c;
    int i = 0;
    int line = 0;

    memset(buf, '\0', BUF_LEN);

    while ((c = fgetc(tmpFile)) != EOF) {
      if (c != '\n') {
        buf[i++] = c;
      } else {
        line++;
        if (line > 4 && strstr(buf, "End of") == NULL) {
          fprintf(destFile, "%s\n", buf);
        }
        memset(buf, '\0', i);
        i = 0;
      }
    }

    fclose(tmpFile);
    fclose(destFile);

    int rv = remove("temp.gitignore");
    if (rv == -1) {
      printf("Error: unable to delete temp.gitignore\n");
    }
  }

  return 0;
}