/*
 * Copyright (C) 2025 spekie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <time.h>
#define ARRAY_MIN (1024)
#define ARRAY_MAX (4096*4096)

int x[ARRAY_MAX];

double get_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void label(int i) {
    if (i < 1e3)
        printf("%1dB,", i);
    else if (i < 1e6)
        printf("%1dK,", i / 1024);
    else if (i < 1e9)
        printf("%1dM,", i / 1048576);
    else
        printf("%1dG,", i / 1073741824);
}

int main(int argc, char* argv[]) {
    int nextstep, i, index, stride;
    int csize;
    double steps, tsteps;
    double loadtime, lastsec, sec0, sec1, sec;

    printf(" ,");
    for (stride = 1; stride <= ARRAY_MAX / 2; stride = stride * 2)
        label(stride * sizeof(int));
    printf("\n");

    for (csize = ARRAY_MIN; csize <= ARRAY_MAX; csize = csize * 2) {
        label(csize * sizeof(int));
        for (stride = 1; stride <= csize / 2; stride = stride * 2) {
            for (index = 0; index < csize; index = index + stride)
                x[index] = index + stride;
            x[index - stride] = 0;
            lastsec = get_seconds();
            sec0 = get_seconds();
            while (sec0 == lastsec)
                sec0 = get_seconds();
            steps = 0.0;
            nextstep = 0;
            sec0 = get_seconds();
            do {
                for (i = stride; i != 0; i = i - 1) {
                    nextstep = 0;
                    do {
                        nextstep = x[nextstep];
                    } while (nextstep != 0);
                }
                steps = steps + 1.0;
                sec1 = get_seconds();
            } while ((sec1 - sec0) < 20.0);
            sec = sec1 - sec0;
            tsteps = 0.0;
            sec0 = get_seconds();
            do {
                for (i = stride; i != 0; i = i - 1) {
                    index = 0;
                    do {
                        index = index + stride;
                    } while (index < csize);
                }
                tsteps = tsteps + 1.0;
                sec1 = get_seconds();
            } while (tsteps < steps);
            sec = sec - (sec1 - sec0);
            loadtime = (sec * 1e9) / (steps * csize);
            printf("%4.1f,", (loadtime < 0.1) ? 0.1 : loadtime);
        }
        printf("\n");
    }
    return 0;
}
