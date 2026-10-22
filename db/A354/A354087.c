/*
 * A354087  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that shares a factor with a(n-1) and whose binary
 *          expansion has a single 1-bit in common with the binary expansion
 *          of a(n-1).
 * https://oeis.org/A354087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A354087
    a(1) = 1;
    for n > 1, a(n) is the smallest unused positive integer that
    shares a non‑trivial common factor with a(n‑1) and whose binary
    expansion has exactly one 1‑bit in common with the binary
    expansion of a(n‑1).  The function below computes a(n) for any
    n ≥ 1 in order‑independent calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"   /* gcd(seq_t, seq_t) provided by the test harness */

/* static state to memoise the sequence and the used flag array */
static seq_t *seq_arr = NULL;    /* sequence values, 1‑based with 0 offset */
static size_t seq_len = 0;
static size_t seq_cap = 0;

static unsigned char *used = NULL;  /* used[i]==1 iff i has appeared */
static size_t used_cap = 0;

/* Ensure that used[] can index at least idx (idx is a value, not a
   position) */
static void ensure_used_cap(size_t idx) {
    if (used_cap > idx) return;
    size_t new_cap = used_cap ? used_cap << 1 : 1024;
    while (new_cap <= idx) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) abort();
    if (new_cap > used_cap)
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that seq_arr can hold at least new_len entries */
static void ensure_seq_cap(size_t new_len) {
    if (seq_cap >= new_len) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1024;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* popcount for any unsigned seq_t type */
static unsigned popcount_seq(seq_t x) {
    unsigned cnt = 0;
    while (x) {
        x &= x - 1;              /* drop lowest set bit */
        ++cnt;
    }
    return cnt;
}

/* trial division: collect distinct prime factors of x into buf */
static void prime_factors(seq_t x, uint64_t *buf, size_t *cnt) {
    *cnt = 0;
    seq_t temp = x;
    for (uint64_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % p == 0) {
            buf[(*cnt)++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && *cnt < 64) buf[(*cnt)++] = (uint64_t)temp;
}

/* -------------------------------------------------------------- */
/* Main function required by the judge                            */
/* -------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* initialise on first call */
    if (!seq_arr) {
        seq_cap = 1024;
        seq_arr = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        used_cap = 1024;
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        seq_arr[0] = 1;                         /* a(1) = 1 */
        ensure_used_cap(1);
        used[1] = 1;
        seq_len = 1;
    }

    /* generate terms up to position n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_arr[seq_len - 1];
        uint64_t primes[64];
        size_t pnum;
        prime_factors(prev, primes, &pnum);

        seq_t best = 0;
        bool first = true;

        for (size_t i = 0; i < pnum; ++i) {
            uint64_t p = primes[i];
            seq_t cand = (seq_t)p;

            while (1) {
                size_t idx = (size_t)cand;
                if (idx >= used_cap) ensure_used_cap(idx);
                if (!used[idx] && popcount_seq(prev & cand) == 1)
                    break;
                cand += p;
            }

            if (first || cand < best) {
                best = cand;
                first = false;
            }
        }

        /* safety fallback (should not be needed) */
        if (first) {
            seq_t cand = 2;
            while (1) {
                size_t idx = (size_t)cand;
                if (idx >= used_cap) ensure_used_cap(idx);
                if (!used[idx] && popcount_seq(prev & cand) == 1)
                    break;
                ++cand;
            }
            best = cand;
        }

        /* store the new term */
        ensure_seq_cap(seq_len + 1);
        seq_arr[seq_len] = best;
        size_t idx = (size_t)best;
        if (idx >= used_cap) ensure_used_cap(idx);
        used[idx] = 1;
        ++seq_len;
    }

    return seq_arr[n - 1];
}
