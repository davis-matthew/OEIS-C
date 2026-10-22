/*
 * A400524  The exponent of the highest power of 2 dividing the number
 *          following the n-th squarefree number.
 * https://oeis.org/A400524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = v2( S(n) + 1 ), where S(n) is the n‑th squarefree number
 *            (i.e. exponent of the highest power of 2 dividing the number
 *            following the n‑th squarefree number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return value for the n-th term (index starts at 1). */
seq_t a(idx_t n)
{
    /* Static storage for already generated squarefree numbers. */
    static uint64_t *sqf = NULL;   /* array of squarefree numbers   */
    static size_t   sqf_len = 0;  /* number of entries currently stored */
    static size_t   sqf_cap = 0;  /* allocated capacity                */

    /* Ensure we have generated at least n squarefree numbers. */
    while (sqf_len < n) {
        uint64_t candidate;
        if (sqf_len == 0)
            candidate = 1;
        else
            candidate = sqf[sqf_len - 1] + 1;

        /* Find next squarefree candidate. */
        for (;;) {
            bool is_sqf = true;
            for (uint64_t i = 2; i * i <= candidate; ++i) {
                if (candidate % (i * i) == 0) {
                    is_sqf = false;
                    break;
                }
            }
            if (is_sqf)
                break;
            ++candidate;
        }

        /* Store the new squarefree number, expanding storage if needed. */
        if (sqf_len == sqf_cap) {
            size_t new_cap = (sqf_cap == 0) ? 8 : sqf_cap * 2;
            void *tmp = realloc(sqf, new_cap * sizeof(uint64_t));
            if (!tmp) /* allocation failure; abort silently (out of memory) */
                return (seq_t)0;
            sqf = (uint64_t *)tmp;
            sqf_cap = new_cap;
        }
        sqf[sqf_len++] = candidate;
    }

    /* Compute v2 of (S(n) + 1). */
    uint64_t s = sqf[n - 1];
    uint64_t m = s + 1;
    unsigned exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp;
    }
    return (seq_t)exp;
}
