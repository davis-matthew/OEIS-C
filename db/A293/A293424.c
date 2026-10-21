/*
 * A293424  Hamming distance between two consecutive semiprimes.
 * https://oeis.org/A293424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Hamming distance between the nth and (n+1)th semiprime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hamming
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hamming.h"

/* ----------------------------------------------------------- */
/*  Static state                                            */
/* ----------------------------------------------------------- */
static uint32_t *spf = NULL;     /* least prime factor array          */
static size_t   spf_limit = 0;   /* size of spf array                 */
static uint64_t current = 4;     /* next integer to test as semiprime */
static uint64_t last_semiprime = 0;   /* previous semiprime found          */
static uint32_t *dist = NULL;    /* stored Hamming distances (uint32) */
static size_t   dist_len = 0;    /* number of distances stored        */
static size_t   dist_cap = 0;    /* current allocation capacity       */
static bool     initialized = false;   /* one‑time init flag               */

/* ----------------------------------------------------------- */
/*  Build least‑prime‑factor sieve up to a given limit       */
/* ----------------------------------------------------------- */
static void build_spf(size_t limit)
{
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= limit; ++i)
        if (spf[i] == i)
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
    spf_limit = limit;
}

/* ----------------------------------------------------------- */
/*  Ensure the distance buffer has enough capacity          */
/* ----------------------------------------------------------- */
static void ensure_dist_capacity(size_t needed)
{
    if (dist_cap >= needed) return;
    size_t new_cap = dist_cap ? dist_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(dist, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    dist = tmp;
    dist_cap = new_cap;
}

/* ----------------------------------------------------------- */
/*  Test whether a number is semiprime                       */
/* ----------------------------------------------------------- */
static inline bool is_semiprime(uint64_t x)
{
    if (x <= 1 || x > spf_limit) return false;
    uint32_t p = spf[(size_t)x];
    if (p == x) return false;        /* prime */
    uint64_t q = x / p;
    if (q <= spf_limit) return spf[(size_t)q] == (uint32_t)q;
    return false;
}

/* ----------------------------------------------------------- */
/*  Hamming distance of two 64‑bit integers                  */

/* ----------------------------------------------------------- */
/*  Public function: a(idx_t n)                              */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, guard only */

    if (!initialized) {
        /* 2 million is safely above the 10 000th semiprime */
        build_spf(2000000);
        initialized = true;
    }

    while (dist_len < (size_t)n) {
        /* find next semiprime */
        while (!is_semiprime(current)) ++current;

        if (last_semiprime != 0) {
            uint32_t d = hamming(last_semiprime, current);
            ensure_dist_capacity(dist_len + 1);
            dist[dist_len++] = d;
        }

        last_semiprime = current;
        ++current;
    }

    return (seq_t)dist[n - 1];
}
