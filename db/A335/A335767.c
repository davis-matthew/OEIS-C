/*
 * A335767  Numbers m such that m = S_af(m) = af(d_1)+af(d_2)+...+af(d_k)
 *          where d_1 d_2 ... d_n is the decimal expansion of m and af(m) =
 *          m!-(m-1)!+(m-2)!+...1! (alternating factorial) with S_af(0) = 1.
 * https://oeis.org/A335767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th integer m such that
   m = S_af(m), where S_af(m) = Σ af(d) over the decimal digits d of m.
   The alternating factorial af(d) is defined as
        af(0) = 1
        af(d) = d! - (d‑1)! + (d‑2)! - … + 1!   for d=1,…,9
   The values of af(d) for d=0…9 are:
        [1, 1, 1, 5, 19, 101, 619, 4381, 35899, 326981].
   The sequence contains only the four terms 1, 621, 643, 398804. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* precomputed af(d) for digits 0..9 */
static const seq_t af_digit[10] = {
    (seq_t)1,   /* af(0) */
    (seq_t)1,   /* af(1) */
    (seq_t)1,   /* af(2) */
    (seq_t)5,   /* af(3) */
    (seq_t)19,  /* af(4) */
    (seq_t)101, /* af(5) */
    (seq_t)619, /* af(6) */
    (seq_t)4381,/* af(7) */
    (seq_t)35899,/* af(8) */
    (seq_t)326981/* af(9) */
};

seq_t a(idx_t n)
{
    /* static storage for the list of solutions */
    static bool initialized = false;
    static seq_t *solutions = NULL;
    static size_t sol_size = 0;
    static size_t sol_cap = 0;

    if (!initialized) {
        /* compute all solutions up to the known bound */
        const idx_t MAX_LIMIT = 2000000; /* higher than 1599999 */
        for (idx_t m = 1; m <= MAX_LIMIT; ++m) {
            idx_t temp = m;
            seq_t sum = (seq_t)0;
            /* include digit 0 if present */
            if (temp == 0) {
                sum += af_digit[0];
            } else {
                while (temp > 0) {
                    unsigned d = (unsigned)(temp % 10);
                    sum += af_digit[d];
                    temp /= 10;
                }
            }
            if (sum == (seq_t)m) {
                /* store solution */
                if (sol_size == sol_cap) {
                    size_t new_cap = sol_cap ? sol_cap << 1 : 4;
                    seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* if memory allocation fails, abort with zero return */
                        return (seq_t)0;
                    }
                    solutions = tmp;
                    sol_cap = new_cap;
                }
                solutions[sol_size++] = (seq_t)m;
            }
        }
        initialized = true;
    }

    if (n == (idx_t)0 || n > sol_size)
        return (seq_t)0;           /* no such element */
    return solutions[(size_t)(n - 1)];
}
