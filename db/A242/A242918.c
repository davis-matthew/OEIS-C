/*
 * A242918  Positions of smaller of twin primes in A001359 with index 5.
 * https://oeis.org/A242918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search; a(n) = n-th k such that pi(A001359(k)) -
 *            pi(A001359(k-1)) = 5, i.e. the index of the twin pair (number of
 *            primes in (A001359(k-1), A001359(k)]) is 5
 * Range:     u8 n <= 17, u16 n <= 4564, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 70 terms through n = 4977 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a242918_res = NULL;
static size_t a242918_cnt = 0;
static uint64_t a242918_lim = 0;

static void a242918_build(uint64_t lim) {
    uint64_t top = lim + 2, i, j;
    unsigned char *comp = (unsigned char *)malloc((size_t)(top + 1));
    size_t cap = 1024, c = 0;
    uint64_t *list;
    uint64_t twin_pos = 0, since = 0;
    bool have_prev = false;

    memset(comp, 0, (size_t)(top + 1));
    comp[0] = comp[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;

    list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        since++;
        if (!comp[i + 2]) {
            twin_pos++;
            if (have_prev && since == 5) {
                if (c == cap) {
                    cap *= 2;
                    list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
                }
                list[c++] = twin_pos;
            }
            have_prev = true;
            since = 0;
        }
    }
    free(comp);
    free(a242918_res);
    a242918_res = list;
    a242918_cnt = c;
    a242918_lim = lim;
}

seq_t a(idx_t n) {
    while ((uint64_t)a242918_cnt < (uint64_t)n) {
        uint64_t nl = a242918_lim ? a242918_lim * 2 : 65536;
        a242918_build(nl);
    }
    return (seq_t)a242918_res[n - 1];
}
