/*
  This is the generator script for the world conquer game, each km of the world is mapped to a long / latitude.
  Each chunk of 1km has a secret password for conquering it hashed with SHA-1
*/
// gcc worldgen.c -lssl -lcrypto -lm -O3 -o worldgen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <openssl/sha.h>

// Earth's radius in km
#define R 6371

// Convert degrees to radians
double deg2rad(double deg) {
  return deg * (M_PI / 180);
}

// Convert radians to degrees
double rad2deg(double rad) {
  return rad * (180 / M_PI);
}

// Calculate the distance between two points on Earth's surface
double distance(double lat1, double lon1, double lat2, double lon2) {
  double dlat = deg2rad(lat2 - lat1);
  double dlon = deg2rad(lon2 - lon1);
  double a = sin(dlat / 2) * sin(dlat / 2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dlon / 2) * sin(dlon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = R * c;
  return d;
}

// Generate a random string of 6 lowercase letters
char* random_string() {
  char* s = malloc(7 * sizeof(char));
  for (int i = 0; i < 6; i++) {
    s[i] = 'a' + rand() % 26;
  }
  s[6] = '\0';
  return s;
}

// Calculate the SHA-1 hash of a string
char* sha1_hash(char* s) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1((unsigned char*)s, strlen(s), hash);
  char* h = malloc(41 * sizeof(char));
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    sprintf(h + i * 2, "%02x", hash[i]);
  }
  h[40] = '\0';
  return h;
}

// Define a structure to store the lat, lon, string and hash
typedef struct {
  double lat;
  double lon;
  char* str;
  char* hash;
} chunk;

// Divide the world in km and map the corresponding lat and lon to an array of chunks
void divide_world(int km) {
  // Loop through the latitudes from -90 to 90
  for (double lat = -90; lat <= 90; lat += km / (double)R) {
    // Calculate the circumference of the circle at this latitude
    double c = 2 * M_PI * R * cos(deg2rad(lat));

    // Loop through the longitudes with the appropriate step size
    for (double lon = -180; lon < 180; lon += km / c * 360) {
      // Create a new chunk
      chunk ch;
      ch.lat = lat;
      ch.lon = lon;
      ch.str = random_string();
      ch.hash = sha1_hash(ch.str);
      printf("%.2f,%.2f,%s\n", ch.lat, ch.lon, ch.hash);
    }
  }
}

int main() {
  // Set the seed for random number generation
  srand(time(NULL));

  // Set the km value
  int km = 1;

  // Divide the world
  divide_world(km);

  return 0;
}

