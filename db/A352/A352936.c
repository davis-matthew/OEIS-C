/*
 * A352936  Records in A121216.
 * https://oeis.org/A352936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   records (left-to-right maxima) of A121216, where A121216:
 *            a(1)=1, a(2)=2, a(n)=smallest unused positive integer coprime to
 *            a(n-2); computed by direct simulation
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9146 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static uint64_t *rec_v = NULL;
static size_t rec_len = 0, rec_cap = 0;
static unsigned char *used_f = NULL;
static size_t used_cap_f = 0;
static uint64_t lo_f = 1, prev_x = 0, prev_y = 0, max_f = 0;
static bool init_f = false;

static uint64_t rec_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void rec_push(uint64_t v)
{
    if (rec_len == rec_cap) {
        rec_cap = rec_cap ? rec_cap * 2 : 64;
        rec_v = (uint64_t *)realloc(rec_v, rec_cap * sizeof(uint64_t));
    }
    rec_v[rec_len++] = v;
}

static void rec_ensure(size_t idx)
{
    if (idx < used_cap_f) return;
    size_t nc = used_cap_f ? used_cap_f : 64;
    while (nc <= idx) nc *= 2;
    used_f = (unsigned char *)realloc(used_f, nc);
    for (size_t i = used_cap_f; i < nc; ++i) used_f[i] = 0;
    used_cap_f = nc;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!init_f) {
        init_f = true;
        rec_ensure(2);
        used_f[1] = 1; used_f[2] = 1;
        prev_x = 1; prev_y = 2; max_f = 2; lo_f = 3;
        rec_push(1); rec_push(2);
    }
    while (rec_len < (size_t)n) {
        uint64_t c;
        for (;;) {
            rec_ensure((size_t)lo_f);
            if (!used_f[lo_f]) break;
            ++lo_f;
        }
        c = lo_f;
        for (;;) {
            rec_ensure((size_t)c);
            if (!used_f[c] && rec_gcd(c, prev_x) == 1) break;
            ++c;
        }
        used_f[c] = 1;
        prev_x = prev_y;
        prev_y = c;
        if (c > max_f) { max_f = c; rec_push(c); }
    }
    return (seq_t)rec_v[(size_t)(n - 1)];
}
