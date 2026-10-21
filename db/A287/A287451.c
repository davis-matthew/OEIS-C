/*
 * A287451  Start with 0 and repeatedly substitute 0->012, 1->201, 2->120.
 * https://oeis.org/A287451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A287451: fixed point of the morphism
 *   0 → 012, 1 → 201, 2 → 120
 * starting from the single symbol 0.
 * a(n) is the n-th character of the infinite sequence
 * (1‑based indexing, a(1)=0, a(2)=1, a(3)=2, …). */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

/* static cache holding the sequence expansion */
static seq_t *seq = NULL;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safe guard for out‑of‑range */

    /* initialise cache if this is the first call */
    if (seq_len == 0) {
        seq_len = 1;
#ifdef __cplusplus
        seq = new seq_t[seq_len];
#else
        seq = (seq_t*)malloc(seq_len * sizeof(seq_t));
#endif
        if (!seq) return (seq_t)0;        /* allocation failure */
        seq[0] = (seq_t)0;
    }

    /* expand the sequence until the requested index is available */
    while (seq_len < n) {
        size_t old_len = (size_t)seq_len;
        size_t new_len = old_len * 3;     /* each symbol expands to 3 */

        seq_t *new_seq;
#ifdef __cplusplus
        new_seq = new seq_t[new_len];
#else
        new_seq = (seq_t*)malloc(new_len * sizeof(seq_t));
#endif
        if (!new_seq) {
#ifdef __cplusplus
            delete[] seq;
#else
            free(seq);
#endif
            return (seq_t)0;                /* allocation failure */
        }

        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            seq_t val = seq[i];
            if (val == (seq_t)0) {
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)2;
            } else if (val == (seq_t)1) {
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            } else { /* val == 2 */
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)0;
            }
        }

#ifdef __cplusplus
        delete[] seq;
#else
        free(seq);
#endif
        seq = new_seq;
        seq_len = (idx_t)new_len;
    }

    return seq[n - 1];   /* 0‑based indexing into the array */
}
