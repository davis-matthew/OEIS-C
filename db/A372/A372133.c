/*
 * A372133  Prime shadow (A181819) with shifted e_i to generate odd numbers,
 *          multiplied by 2^(A064839 - 1).
 * https://oeis.org/A372133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A372133 (Prime shadow) using prime signature. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* first few primes, 1‑based indexing */
static const uint32_t primes_by_index[201] = {
    0,                    /* unused 0 */
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
    547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
    607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
    661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
    739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
    811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
    947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013,
    1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
    1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
    1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223
};

#define HASH_SIZE (1U << 17)   /* 131072 entries, ample for n ≤ 65536 */

typedef struct {
    uint64_t key;
    uint32_t count;
} SigEntry;

static SigEntry sig_map[HASH_SIZE];
static bool sig_map_inited = false;

/* simple 64‑bit multiplicative hash */
static inline uint64_t hash64(uint64_t x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

static void sig_map_init(void)
{
    if (sig_map_inited) return;
    for (size_t i = 0; i < HASH_SIZE; ++i) {
        sig_map[i].key = 0;
        sig_map[i].count = 0;
    }
    sig_map_inited = true;
}

static uint32_t sig_map_inc(uint64_t key)
{
    sig_map_init();
    size_t h = hash64(key) & (HASH_SIZE - 1);
    while (true) {
        if (sig_map[h].key == 0) {        /* empty slot */
            sig_map[h].key = key;
            sig_map[h].count = 1;
            return 1;
        }
        if (sig_map[h].key == key) {      /* same signature */
            sig_map[h].count++;
            return sig_map[h].count;
        }
        h = (h + 1) & (HASH_SIZE - 1);
    }
}

/* cache of already computed terms */
static seq_t *cache = NULL;
static idx_t  cache_n = 0;   /* cache valid for 1 .. cache_n */

/* factor n into exponents (small prime list). */
static void factor_exponents(uint64_t n, int exps[], int *cnt)
{
    uint64_t tmp = n;
    int ecnt = 0;
    for (size_t pi = 1; pi <= 200 && (uint64_t)primes_by_index[pi] * (uint64_t)primes_by_index[pi] <= tmp; ++pi) {
        uint32_t p = primes_by_index[pi];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            exps[ecnt++] = e;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        exps[ecnt++] = 1;
    }
    *cnt = ecnt;
}

/* insertion sort for small arrays */
static void sort_exps(int exps[], int cnt)
{
    for (int i = 1; i < cnt; ++i) {
        int key = exps[i];
        int j = i - 1;
        while (j >= 0 && exps[j] > key) {
            exps[j + 1] = exps[j];
            --j;
        }
        exps[j + 1] = key;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of OEIS range */
    if (n == 1) return 0;          /* a(1) = 0 */

    /* grow cache as needed */
    if (n > cache_n) {
        size_t new_cap = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else if (new_cap > (size_t)cache_n) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }

        /* compute missing terms sequentially */
        for (idx_t m = cache_n + 1; m <= n; ++m) {
            uint64_t mm = (uint64_t)m;
            int exps[16];
            int exps_cnt = 0;

            factor_exponents(mm, exps, &exps_cnt);
            sort_exps(exps, exps_cnt);

            /* build 4‑bit packed key of exponents, ascending */
            uint64_t key = 0;
            for (int i = 0; i < exps_cnt; ++i) {
                key = (key << 4) | (uint64_t)exps[i];
            }

            uint32_t occ = sig_map_inc(key);     /* occurrence count */

            /* product of primes(e+1) */
            seq_t prod = 1;
            for (int i = 0; i < exps_cnt; ++i) {
                prod *= (seq_t)primes_by_index[exps[i] + 1];
            }
            seq_t odd = prod - (seq_t)2;
            seq_t mult = pow2((idx_t)(occ - 1));
            cache[m - 1] = odd * mult;
        }

        cache_n = n;
    }

    return cache[n - 1];
}
