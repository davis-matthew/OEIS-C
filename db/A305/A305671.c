/*
 * A305671  Most common value of sigma (A000203) among all composites
 *          (A073255) up to composite(n) = A002808(n) inclusive, or 0 if there
 *          is a tie.
 * https://oeis.org/A305671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = most common value of sigma (A000203) among all composites
 *            up to composite(n) (A002808(n)), or 0 if there is a tie
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static state retaining progress between calls */
    static idx_t processed_composites = 0;   /* how many composites processed so far */
    static idx_t next_candidate = 4;         /* next integer to check for composite */
    static uint32_t best_sigma = 0;          /* sigma value with highest frequency so far */
    static uint32_t best_count = 0;          /* frequency of best_sigma */
    static uint32_t best_freq_count = 0;     /* number of sigma values tied for best_count */

    static uint32_t *freq = NULL;           /* frequency table indexed by sigma value */
    static size_t freq_cap = 0;              /* current capacity of freq array */

    static seq_t *answers = NULL;           /* a(n) values for processed composites (1‑based) */
    static size_t ans_cap = 0;              /* capacity of answers array */

    /* Extend state until we have processed at least n composites */
    while (processed_composites < n)
    {
        /* Find the next composite number */
        while (!((next_candidate >= 4) && (!is_prime(next_candidate)))) {
            next_candidate++;
        }
        uint32_t num = (uint32_t)next_candidate;

        /* Compute sigma(num) using an O(√num) loop */
        uint64_t sigma64 = 1ULL + num;          /* 1 and num themselves */
        for (uint32_t d = 2; (uint64_t)d * d <= num; ++d)
        {
            if (num % d == 0)
            {
                uint32_t other = num / d;
                if (d == other)
                    sigma64 += d;
                else
                    sigma64 += d + other;
            }
        }
        uint32_t sigma_val = (uint32_t)sigma64;  /* sigma fits into 32 bits for the range needed */

        /* Ensure freq table can index sigma_val */
        if (sigma_val >= freq_cap)
        {
            size_t new_cap = freq_cap ? freq_cap * 2 : 256;
            while (new_cap <= sigma_val) new_cap <<= 1;
            uint32_t *new_arr = (uint32_t *)realloc(freq, new_cap * sizeof(uint32_t));
            if (!new_arr) exit(1);
            if (new_cap > freq_cap) /* zero the newly added slots */
                memset(new_arr + freq_cap, 0, (new_cap - freq_cap) * sizeof(uint32_t));
            freq = new_arr;
            freq_cap = new_cap;
        }

        /* Update frequency for this sigma value */
        uint32_t newcnt = ++freq[sigma_val];
        if (newcnt > best_count)
        {
            best_count = newcnt;
            best_sigma = sigma_val;
            best_freq_count = 1;
        }
        else if (newcnt == best_count)
        {
            best_freq_count++; /* another sigma tied for the highest count */
        }

        /* Ensure answers array has space for this entry */
        if (processed_composites >= ans_cap)
        {
            size_t new_cap = ans_cap ? ans_cap * 2 : 16;
            seq_t *new_arr = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
            if (!new_arr) exit(1);
            answers = new_arr;
            ans_cap = new_cap;
        }

        /* Record the a(n) value for this composite */
        answers[processed_composites] = (best_freq_count == 1) ? (seq_t)best_sigma : 0;

        /* Prepare for next iteration */
        processed_composites++;
        next_candidate++;
    }

    return answers[n - 1];
}
