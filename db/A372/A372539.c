/*
 * A372539  Numbers k such that the number of ones minus the number of zeros
 *          in the binary expansion of the k-th prime number is -1.
 * https://oeis.org/A372539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that (#ones – #zeros) of the k-th
 *            prime's binary * expansion equals –1. The function computes the
 *            sequence by generating primes one by one * and testing the
 *            binary property
 * Range:     u8 n <= 35, u16 n <= 4350, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* count set bits in a 64‑bit integer */
static idx_t popcount_uint64(uint64_t x) {
    idx_t c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* number of bits needed to represent x (x>0) */
static idx_t bitlen_uint64(uint64_t x) {
    idx_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* simple Sieve of Eratosthenes up to 'limit'.  Returns array of primes and count. */
static void sieve_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out   = 0;
        return;
    }
    bool *is_comp = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out   = 0;
        return;
    }
    size_t est = limit / 10 + 10;
    uint64_t *pr = (uint64_t*)malloc(est * sizeof(uint64_t));
    size_t prcnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prcnt == est) {
                est *= 2;
                pr = (uint64_t*)realloc(pr, est * sizeof(uint64_t));
            }
            pr[prcnt++] = (uint64_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    *primes_out = pr;
    *cnt_out    = prcnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined, but safe */
    static uint64_t *prime_list = NULL;       /* primes up to sieve_limit */
    static size_t prime_cnt   = 0;            /* number of primes in prime_list */
    static idx_t sieve_limit  = 0;            /* current sieve limit (value) */
    static idx_t processed_index = 0;         /* how many primes have been inspected */

    static seq_t *seq_buf  = NULL;            /* stored a(k) values */
    static size_t seq_len  = 0, seq_cap = 0;   /* current length / capacity */

    /* build sequence until we have at least n elements */
    while (seq_len < n) {
        /* need more primes?  extend the sieve if exhausted */
        while (processed_index >= prime_cnt) {
            if (sieve_limit < 1000000) sieve_limit = 1000000;
            else sieve_limit *= 2;

            uint64_t *new_list = NULL;
            size_t    new_cnt  = 0;
            sieve_primes_upto(sieve_limit, &new_list, &new_cnt);
            if (prime_list) free(prime_list);
            prime_list = new_list;
            prime_cnt  = new_cnt;
        }

        uint64_t p = prime_list[processed_index];
        idx_t    k = processed_index + 1;         /* 1‑based index of this prime */

        idx_t ones = popcount_uint64(p);
        idx_t len  = bitlen_uint64(p);

        /* property: ones - zeros == -1  <=> 2*ones == len - 1 */
        if (2 * ones == len - 1) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_buf = (seq_t*)realloc(seq_buf, seq_cap * sizeof(seq_t));
            }
            seq_buf[seq_len++] = (seq_t)k;
        }
        ++processed_index;
    }
    return seq_buf[n - 1];
}
