/*
 * A361123  1 if n-th composite number A002808(n) is divisible by 3, otherwise
 *          0.
 * https://oeis.org/A361123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n‑th composite number is divisible by 3, 0
 *            otherwise
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* static state to remember already found composites */
    static uint64_t last_checked = 3;             /* last number tested for compositeness */
    static uint64_t *composites = NULL;           /* array of found composites */
    static size_t composites_cnt = 0;             /* number of composites stored */
    static size_t composites_cap = 0;             /* allocated capacity */

    /* generate composites until we have at least n of them */
    while (composites_cnt < n) {
        last_checked++;                          /* next candidate number */
        if (!is_prime((idx_t)last_checked)) {    /* composite? */
            /* enlarge array if necessary */
            if (composites_cnt >= composites_cap) {
                size_t new_cap = composites_cap ? composites_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;      /* allocation failure – give up */
                composites = tmp;
                composites_cap = new_cap;
            }
            composites[composites_cnt++] = last_checked;
        }
    }

    uint64_t comp = composites[n - 1];
    return (seq_t)((comp % 3) == 0);
}
