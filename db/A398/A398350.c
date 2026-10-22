/*
 * A398350  Primes p that start a run of three consecutive primes in
 *          arithmetic progression (CPAP-3) such that the two primes
 *          immediately preceding p, or the two primes immediately following
 *          the run, form a twin prime pair.
 * https://oeis.org/A398350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p that starts a CPAP‑3 run, with either
 *            the two primes immediately preceding p forming a twin pair or
 *            the two primes immediately following the run forming a twin pair
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   grow_size_t_arr, grow_uint64_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_size_t_arr.h"
#include "../utils/grow_uint64_arr.h"

/* Grow a dynamic array of type seq_t (the sequence value) */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t needed)
{
    size_t newcap = (*cap == 0) ? 64 : *cap;
    while (newcap < needed) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Return the next prime >= start.  A simple trial‑division routine
   using the primes collected so far. */
static uint64_t next_prime_uint64(uint64_t *primes, size_t prime_cnt, uint64_t start)
{
    if (prime_cnt == 0) return 2;
    if (start <= primes[prime_cnt - 1]) start = primes[prime_cnt - 1] + 1;
    if (start % 2 == 0) ++start;
    for (;; start += 2)
    {
        bool composite = false;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p > start / p) break;          /* p*p > start */
            if (start % p == 0) { composite = true; break; }
        }
        if (!composite) return start;
    }
}

/* Main function computing sequence A398350. */
seq_t a(idx_t n)
{
    /* Static state persistent across calls. */
    static uint64_t *primes = NULL;
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;

    static size_t *pending = NULL;
    static size_t pending_cap = 0;
    static size_t pending_cnt = 0;

    static seq_t *answers = NULL;
    static size_t ans_cap = 0;
    static size_t ans_cnt = 0;

    /* Generate terms until we have at least n of them. */
    while (ans_cnt < n)
    {
        /* -------- Generate next prime -------- */
        uint64_t next = next_prime_uint64(primes, prime_cnt,
                                         (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1);

        if (prime_cnt == prime_cap) grow_uint64_arr(&primes, &prime_cap, prime_cnt + 1);
        primes[prime_cnt++] = next;

        /* -------- Check for a new CPAP‑3 ending at the new prime -------- */
        if (prime_cnt >= 3)
        {
            size_t idx = prime_cnt - 1;                      /* index of the new prime r */
            uint64_t p0 = primes[idx - 2];                  /* p   */
            uint64_t p1 = primes[idx - 1];                  /* q   */
            uint64_t p2 = primes[idx];                      /* r   */
            if (p1 - p0 == p2 - p1)                         /* CPAP‑3 ? */
            {
                size_t start_idx = idx - 2;                  /* index of p */

                /* 1. Check if the two primes immediately preceding p form a twin pair. */
                if (start_idx >= 2 && (primes[start_idx - 1] - primes[start_idx - 2] == 2))
                {
                    if (ans_cnt == ans_cap) grow_seq_arr(&answers, &ans_cap, ans_cnt + 1);
                    answers[ans_cnt++] = (seq_t)primes[start_idx];
                }
                else
                {
                    /* 2. Store for later checking of the trailing twin pair. */
                    if (pending_cnt == pending_cap) grow_size_t_arr(&pending, &pending_cap, pending_cnt + 1);
                    pending[pending_cnt++] = start_idx;
                }
            }
        }

        /* -------- Process pending CPAP‑3s that may now satisfy the trailing twin pair -------- */
        for (size_t i = 0; i < pending_cnt; )
        {
            size_t s = pending[i];                        /* start index of the run */
            if (prime_cnt > s + 4)                        /* we have primes s+3 and s+4 */
            {
                if (primes[s + 4] - primes[s + 3] == 2)   /* trailing twin pair */
                {
                    if (ans_cnt == ans_cap) grow_seq_arr(&answers, &ans_cap, ans_cnt + 1);
                    answers[ans_cnt++] = (seq_t)primes[s];
                }
                /* Remove this entry regardless of qualification (it can never qualify again). */
                pending[i] = pending[--pending_cnt];
            }
            else
            {
                ++i;
            }
        }
    }

    return answers[n - 1];
}
