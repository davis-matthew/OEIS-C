/*
 * A355851  The difference between the indices of the two most recent
 *          instances of a(n-1), not including a(n-1) itself, or 0 if not
 *          enough such instances exist.
 * https://oeis.org/A355851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355851 – The difference between the indices of the two
   most recent instances of a(n‑1) (excluding a(n‑1) itself),
   or 0 if fewer than two such instances exist.  */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *seq  = NULL;      /* stored values */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* capacity of seq[] */

/* Ensure on‑heap storage large enough for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = (seq_cap > 0) ? seq_cap << 1 : 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

/* Return the n‑th term of A355851 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    size_t up_to = (size_t)n;            /* index to fill (1‑based) */

    /* Initialise first term if the array is empty. */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)0;                 /* a(1) = 0 */
        seq_len = 1;
    }

    /* Generate all missing terms up to n. */
    if (seq_len < up_to) {
        ensure_capacity(up_to);
        for (size_t idx = seq_len; idx < up_to; ++idx) { /* idx is 0‑based */
            if (idx == 0) {                   /* already set */
                seq[0] = (seq_t)0;
                continue;
            }

            seq_t prev_val = seq[idx - 1];
            seq_t result   = (seq_t)0;

            if (idx >= 2) {                   /* at least two previous terms exist */
                size_t first = 0, second = 0;
                int  got_first  = 0, got_second = 0;

                size_t i = idx - 2;
                while (1) {
                    if (seq[i] == prev_val) {
                        if (!got_first) {
                            first = i;
                            got_first = 1;
                        } else if (!got_second) {
                            second = i;
                            got_second = 1;
                            break;
                        }
                    }
                    if (i == 0) break;
                    --i;
                }

                if (got_first && got_second) {
                    result = (seq_t)(first - second);   /* distance between indices */
                }
            }

            seq[idx] = result;
        }
        seq_len = up_to;
    }

    return seq[n - 1];   /* array is 0‑based, requested index is 1‑based */
}
