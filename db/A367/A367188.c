/*
 * A367188  a(1) = 1, thereafter a(n) = a(n-1) - A007504(n-1) if positive and
 *          novel, else a(n-1) + A007504(n-1).
 * https://oeis.org/A367188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>1 let s=A007504(n-1)=sum of the first n-1 primes.
 *            a(n)=a(n-1)-s if a(n-1)-s>0 and the result has not appeared
 *            before; otherwise a(n)=a(n-1)+s
 * Range:     u8 n <= 10, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* computes the n-th term of OEIS sequence A367188 */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq   = NULL;     /* array of computed a(k) */
    static size_t seq_len = 0;      /* number of terms stored */
    static size_t seq_cap = 0;      /* capacity of seq array */

    static seq_t *prime_sum = NULL; /* prime_sum[i] = sum of first i primes, prime_sum[0]=0 */
    static size_t prime_sum_len = 0;
    static bool primes_inited = false;

    /* initialise primes and cumulative sums once */
    if (!primes_inited) {
        /* simple sieve up to a fixed bound that covers at least 10000 primes */
        const idx_t sieve_limit = 200000;       /* > 104729 = 10000th prime */
        size_t size = (size_t)sieve_limit + 1;
        bool *is_comp = (bool*)malloc(size);
        for (size_t i = 0; i < size; ++i) is_comp[i] = false;

        size_t *primes = NULL;
        size_t primes_cap = 0;
        size_t prime_cnt = 0;

        for (idx_t p = 2; p <= sieve_limit; ++p) {
            if (!is_comp[p]) {               /* p is prime */
                if (primes_cap <= prime_cnt) {
                    primes_cap = primes_cap ? primes_cap * 2 : 128;
                    primes = (size_t*)realloc(primes, primes_cap * sizeof(size_t));
                }
                primes[prime_cnt++] = (size_t)p;
                if (p * p <= sieve_limit) {
                    for (idx_t m = p * p; m <= sieve_limit; m += p)
                        is_comp[m] = true;
                }
            }
        }
        free(is_comp);

        /* build cumulative sum array */
        prime_sum_len = prime_cnt + 1;
        prime_sum = (seq_t*)malloc(prime_sum_len * sizeof(seq_t));
        prime_sum[0] = (seq_t)0;
        seq_t cum = (seq_t)0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            cum += (seq_t)primes[i];
            prime_sum[i + 1] = cum;
        }
        free(primes);
        primes_inited = true;
    }

    if (n == 0) return (seq_t)0;   /* undefined, but never called */

    /* ensure first term is available */
    if (seq_len == 0) {
        seq_cap = 256;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* extend sequence up to term n */
    while (seq_len < (size_t)n) {
        idx_t cur_n = seq_len + 1;          /* we are computing a(cur_n) */
        idx_t idx = cur_n - 1;              /* need A007504(cur_n-1) */
        seq_t sum_primes = prime_sum[idx];  /* sum of first idx primes */

        seq_t prev = seq[seq_len - 1];
        bool can_subtract = prev > sum_primes;
        seq_t candidate = 0;
        if (can_subtract) {
            candidate = prev - sum_primes;
        }

        bool found = false;
        if (can_subtract) {
            for (size_t j = 0; j < seq_len; ++j) {
                if (seq[j] == candidate) { found = true; break; }
            }
        }

        seq_t next;
        if (can_subtract && !found) {
            next = candidate; /* subtracting is allowed and novel */
        } else {
            next = prev + sum_primes; /* otherwise add */
        }

        /* store the new term */
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = next;
        ++seq_len;
    }

    return seq[seq_len - 1];
}
