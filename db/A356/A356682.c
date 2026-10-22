/*
 * A356682  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 2, 1.
 * https://oeis.org/A356682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest infinite square‑free sequence of nonnegative
   integers beginning with 2, 1.  The definition is:
      a(1)=2, a(2)=1;
      a(n)= the smallest non‑negative integer k such that the
           concatenation a(1),…,a(n−1),k contains no
           square of the form X X, where X is any non‑empty block
           of consecutive terms.  */

/* Standard headers required by the constraints. */
#include <stdlib.h>
#include <stdbool.h>

/* Ensure that the array pointed to by *ptr has room for at least NEW
   elements.  The capacity is doubled until it meets or exceeds NEW.  */
static void
ensure_cap(size_t new_n, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= new_n)
        return;
    size_t ncap = (*cap == 0) ? 4 : (*cap << 1);
    while (ncap < new_n)
        ncap <<= 1;
    *ptr = realloc(*ptr, ncap * elem_size);
    *cap = ncap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static state holding the built part of the sequence. */
    static seq_t            *seq   = NULL;   /* array of terms            */
    static size_t            cap   = 0;      /* allocated capacity        */
    static idx_t             built = 0;      /* number of terms ready     */

    /* Ensure the first two terms exist.  This also handles the case
       where the very first call is for n=1 or n=2. */
    if (built < 2) {
        ensure_cap(2, &cap, (void **)&seq, sizeof(seq_t));
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)1;
        built = 2;
    }

    /* Build more terms until we have at least n of them. */
    while (built < n) {
        idx_t m = built;   /* index of the new term (0‑based) */

        /* Try candidate values in ascending order. */
        for (seq_t cand = 0; ; ++cand) {
            /* Make room for the new value. */
            if (m >= cap)
                ensure_cap(m + 1, &cap, (void **)&seq, sizeof(seq_t));

            seq[m] = cand;           /* tentative assignment */

            /* Check for a square that ends at position m. */
            bool ok = true;
            idx_t max_half = (m + 1) / 2;   /* longest possible half‑block length */
            for (idx_t len = 1; len <= max_half && ok; ++len) {
                idx_t start1 = (m + 1) - 2 * len;   /* first block start */
                idx_t start2 = start1 + len;        /* second block start */
                bool equal = true;
                for (idx_t i = 0; i < len; ++i) {
                    if (seq[start1 + i] != seq[start2 + i]) {
                        equal = false;
                        break;
                    }
                }
                if (equal) {          /* a square X|X has been found */
                    ok = false;
                    break;
                }
            }

            if (ok) {   /* found minimal admissible candidate */
                break;   /* leave candidate loop */
            }
            /* otherwise try the next integer */
        }
        ++built;   /* new term settled */
    }

    /* Return the requested term (1‑based index). */
    return seq[n - 1];
}
