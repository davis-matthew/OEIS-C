/*
 * A275168  Positive integers not of the form x^3 + 3*y^2 + z^2 with x,y,z
 *          nonnegative integers.
 * https://oeis.org/A275168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 145, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A275168: positive integers NOT representable as x^3 + 3*y^2 + z^2
   (x,y,z ≥ 0).  The function builds a table of all such numbers up to
   LIMIT (200 000) by enumerating all representable values and marks
   the remaining integers.  For any required index n it returns the
   (n‑th) element of that table.  No closed formula is known. */

#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 200000U          /* Upper bound containing all known terms */

/* Integer floor square root for unsigned values up to LIMIT. */
static unsigned
isqrt(unsigned n)
{
    unsigned r = 0;
    /* r will never exceed √n (≤ 447 for LIMIT = 200 000),
       so the multiplication below cannot overflow 64 bits.   */
    while ((unsigned)((unsigned long long)(r + 1) * (r + 1)) <= n) {
        ++r;
    }
    return r;
}

/* Pre‑compute all numbers not of the form x^3 + 3*y^2 + z^2 up to LIMIT */
static void
_init_nonrep(seq_t **out, idx_t *cnt)
{
    static bool bitmap[LIMIT + 1] = {0};
    static bool done = false;

    if (done)
        return;

    /* Mark all representable values. */
    for (unsigned x = 0; ; ++x) {
        unsigned x3 = x * x * x;
        if (x3 > LIMIT)
            break;

        unsigned remaining_after_x = LIMIT - x3;
        unsigned a_div3 = remaining_after_x / 3;
        unsigned y_max = isqrt(a_div3);

        for (unsigned y = 0; y <= y_max; ++y) {
            unsigned y_term = 3 * y * y;
            unsigned rem = remaining_after_x - y_term;
            unsigned z_max = isqrt(rem);

            for (unsigned z = 0; z <= z_max; ++z) {
                unsigned val = x3 + y_term + z * z;
                bitmap[val] = true;
            }
        }
    }

    /* Collect the non‑representable numbers into a dynamic array. */
    size_t cap = 256;
    size_t len = 0;
    seq_t *res = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!res)
        return;               /* Allocation failure – should not happen. */

    for (unsigned i = 1; i <= LIMIT; ++i) {
        if (!bitmap[i]) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(res, cap * sizeof(seq_t));
                if (!tmp) {
                    free(res);
                    return;
                }
                res = tmp;
            }
            res[len++] = (seq_t)i;
        }
    }

    *out = res;
    *cnt = (idx_t)len;
    done = true;
}

/* Required OEIS function. */
seq_t a(idx_t n)
{
    static seq_t *nonrep = NULL;
    static idx_t nonrep_cnt = 0;

    if (!nonrep)
        _init_nonrep(&nonrep, &nonrep_cnt);

    if (n == 0 || n > nonrep_cnt)
        return (seq_t)0;    /* undefined for out‑of‑range indices */

    return nonrep[n - 1];
}
