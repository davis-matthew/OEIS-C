/*
 * A338699  Sequence A is the primes. Sequence B is the "smallest" sequence of
 *          nondecreasing powers of two whose sum over the first N terms is at
 *          least equal to the sum over the first N terms of Sequence A. a(n)
 *          is the amount by which the sum of the first n terms of Sequence B
 *          exceeds the sum of the first n...
 * https://oeis.org/A338699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A338699
 *  a(n) = excess of the minimal nondecreasing sequence of powers of two
 *  (B) over the primes (A) after n terms.
 *  a(n) = (∑_{i=1..n} b_i) – (∑_{i=1..n} p_i)
 *
 *  The algorithm keeps a running sum of the primes (`sumA`) and a running
 *  sum of the chosen powers of two (`sumB`).  For each new prime `p`
 *  it computes the minimal block `b` such that
 *
 *      sumB + b  ≥  sumA + p
 *
 *  and the nondecreasing constraint `b ≥ prev`.  The smallest
 *  possible `b` is either `prev` (if the inequality already holds)
 *  or the next power of two greater than the required amount.
 *
 *  Example terms:
 *    a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=6, a(6)=9, a(7)=8, a(8)=5,
 *    a(9)=14, a(10)=17, …
 *
 *  The function is memoised so that calls with arbitrary `n`
 *  remain efficient.
 *
 *  Required utilities:  pow2.h, bitlen_seq_t.h, is_prime.h
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime.h"

/* Helper: smallest power of two ≥ x, x > 0 */
static seq_t next_pow2_upper(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)1;
    return pow2((idx_t)bitlen_seq_t(x - (seq_t)1));
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms already computed */

    /* Running state variables */
    static seq_t sumA = 0;          /* cumulative sum of primes               */
    static seq_t sumB = 0;          /* cumulative sum of powers of two         */
    static seq_t prev  = 0;          /* last block used                         */

    /* Prime generation state */
    static idx_t candidate = 2;
    static idx_t *prime_tbl = NULL;
    static size_t prime_cap = 0;
    static size_t prime_len = 0;

    /* Allocate or grow cache if necessary */
    if (n > cache_len) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < (size_t)n) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Generate missing terms */
        for (size_t idx = cache_len; idx < n; ++idx) {  /* idx 0‑based, term = idx+1 */
            /* 1. Find next prime */
            idx_t p;
            for (;;) {
                bool is_p = true;
                for (size_t i = 0; i < prime_len; ++i) {
                    idx_t pr = prime_tbl[i];
                    if (pr * pr > candidate) break;
                    if (candidate % pr == 0) { is_p = false; break; }
                }
                if (is_p) { p = candidate; break; }
                ++candidate;
            }

            /* Store prime for future trial divisions */
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                while (new_cap < prime_len + 1) new_cap <<= 1;
                prime_tbl = (idx_t*)realloc(prime_tbl, new_cap * sizeof(idx_t));
                prime_cap = new_cap;
            }
            prime_tbl[prime_len++] = p;
            ++candidate;   /* next candidate starts after this prime */

            /* 2. Compute required block */
            seq_t diff = sumA + (seq_t)p;           /* sumA_prev + p */
            seq_t block;
            if (sumB >= diff) {                     /* already enough */
                block = prev;
            } else {
                seq_t need = diff - sumB;           /* positive amount needed */
                if (need <= prev) {
                    block = prev;
                } else {
                    block = next_pow2_upper(need);
                    if (block < prev) block = prev;
                }
            }

            /* 3. Update sums and record excess */
            sumB += block;
            sumA = diff;
            seq_t excess = sumB - sumA;
            cache[idx] = excess;

            prev = block;
        }

        cache_len = n;
    }

    return cache[n - 1];
}
