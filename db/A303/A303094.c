/*
 * A303094  Balanced primes of order one ending in 7.
 * https://oeis.org/A303094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced primes of order one ending in 7:
   a(n) is the nth prime p such that p ≡ 7 (mod 10) and 2p = prev(p) + next(p) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Cache of found terms  (p ≡ 7 (mod 10)) */
static seq_t *bal_list   = NULL; /* array of terms */
static size_t bal_len    = 0;    /* number of terms stored */
static size_t bal_cap    = 0;    /* allocated capacity of bal_list */

/* Current window of three consecutive primes while searching */
static bool   bal_init = false;
static uint64_t a_pp = 0, b_pp = 0, c_pp = 0;

/* Public entry point (OEIS definition). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* Initialise the first three primes (2, 3, 5) only once. */
    if (!bal_init) {
        a_pp = 2;
        b_pp = next_prime(a_pp + 1);   /* 3 */
        c_pp = next_prime(b_pp + 1);   /* 5 */
        bal_init = true;
    }

    /* Generate additional primes until we collect n balanced primes. */
    while (bal_len < (size_t)n) {
        /* Check if the middle prime in the window is a balanced prime
           that ends with 7. */
        if (b_pp % 10ULL == 7ULL) {
            unsigned __int128 lhs = (unsigned __int128)b_pp * 2;
            unsigned __int128 rhs = (unsigned __int128)a_pp + c_pp;
            if (lhs == rhs) {
                append_candidate(&bal_list, &bal_len, &bal_cap, (seq_t)b_pp);
            }
        }

        /* Shift the window to the next trio of consecutive primes. */
        a_pp = b_pp;
        b_pp = c_pp;
        c_pp = next_prime(b_pp + 1);
    }

    return bal_list[n - 1];
}
