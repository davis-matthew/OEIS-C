/*
 * A392982  Run lengths of consecutive increases of prime(n)/n where prime(n)
 *          is the n-th prime.
 * https://oeis.org/A392982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Run lengths of consecutive increases of prime(n)/n.
   The algorithm stores the run lengths in static state,
   so that calls may be made in any order.
   It grows the run array on demand and uses 64‑bit
   cross‑products to compare ratios without overflow. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *runs = NULL;      /* array of run lengths */
    static size_t  runs_cnt = 0;    /* how many runs are stored */
    static size_t  runs_cap = 0;    /* current capacity of runs array */

    static idx_t   last_prime_index = 0;  /* 1‑based index of last prime processed */
    static uint64_t last_prime_val  = 0;  /* value of last prime processed */
    static seq_t   cur_run_len      = 0;  /* length of the run currently being built */

    while (runs_cnt < n) {
        idx_t k = last_prime_index + 1;
        uint32_t p32 = get_prime(k);
        uint64_t prime_val = (uint64_t)p32;

        if (last_prime_index == 0) {
            /* first prime – start first run */
            last_prime_val = prime_val;
            last_prime_index = k;
            cur_run_len = 1;
            continue;
        }

        /* Compare prime(last)/last_index  and  prime(k)/k
           using cross‑product to avoid division. */
        uint64_t left  = last_prime_val * (uint64_t)k;
        uint64_t right = prime_val * (uint64_t)last_prime_index;

        if (left > right) {           /* ratio decreased – run ends */
            if (runs_cnt == runs_cap) {
                size_t new_cap = runs_cap ? runs_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                runs = tmp;
                runs_cap = new_cap;
            }
            runs[runs_cnt++] = cur_run_len;
            cur_run_len = 1;          /* start new run */
        } else {
            cur_run_len++;             /* continue current run */
        }

        last_prime_val = prime_val;
        last_prime_index = k;
    }

    return runs[n - 1];
}
