/*
 * A295063  Solution of the complementary equation a(n) = 4*a(n-2) + b(n-1) +
 *          b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, and (a(n)) and (b(n))
 *          are increasing complementary sequences.
 * https://oeis.org/A295063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A295063
   Complementary sequences a(n) and b(n) with
   a(0)=1, a(1)=3, b(0)=2,
   a(n)=4*a(n-2)+b(n-1)+b(n-2)   (n>=2),
   a[] and b[] are increasing complementary sequences.  */

/* The function keeps static arrays that grow as needed.
   For C++ builds it uses new/delete to honour constructors of
   arbitrary‑precision seq_t classes; for C builds it falls back
   to malloc/realloc.  */

#include <stdlib.h>

typedef struct {
    seq_t *data;
    size_t cap;
} SeqArray;

static void seqarray_init(SeqArray *a)
{
    a->data = NULL;
    a->cap  = 0;
}

static void seqarray_resize(SeqArray *a, size_t new_cap)
{
    if (new_cap <= a->cap)
        return;
#ifdef __cplusplus
    seq_t *new_data = new seq_t[new_cap];
    if (a->data) {
        for (size_t i = 0; i < a->cap; ++i)
            new_data[i] = a->data[i];
        delete[] a->data;
    }
    a->data = new_data;
#else
    seq_t *new_data = (seq_t *)realloc(a->data, new_cap * sizeof(seq_t));
    a->data = new_data;
#endif
    a->cap = new_cap;
}

static void seqarray_destroy(SeqArray *a)
{
    if (a->data) {
#ifdef __cplusplus
        delete[] a->data;
#else
        free(a->data);
#endif
        a->data = NULL;
        a->cap  = 0;
    }
}

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static SeqArray a_seq = { NULL, 0 };
    static SeqArray b_seq = { NULL, 0 };
    static idx_t computed_len = 0;      /* number of a indices computed so far */
    static idx_t pa_idx = 0;            /* next a index to inspect when searching b */
    static idx_t pb_idx = 0;            /* next b index to inspect when searching b */

    /* Resize the arrays to hold at least n+1 elements */
    seqarray_resize(&a_seq, (size_t)n + 1);
    seqarray_resize(&b_seq, (size_t)n + 1);

    /* Return cached value if already known */
    if (n < computed_len)
        return a_seq.data[n];

    /* Initialise seed values on first use */
    if (computed_len == 0) {
        a_seq.data[0] = (seq_t)1;
        b_seq.data[0] = (seq_t)2;
        computed_len = 1;          /* a[0] is known */
    }

    /* Compute values up to requested index */
    for (idx_t m = computed_len; m <= n; ++m) {
        if (m == 1) {          /* a(1) is fixed */
            a_seq.data[1] = (seq_t)3;

            /* b(1) : smallest integer > b(0) not in a[0..1] or b[0] */
            seq_t cand = b_seq.data[0] + (seq_t)1;
            for (;;) {
                while (pa_idx <= 1 && a_seq.data[pa_idx] < cand)
                    ++pa_idx;
                if (pa_idx <= 1 && a_seq.data[pa_idx] == cand) {
                    ++cand;
                    continue;
                }
                while (pb_idx < 1 && b_seq.data[pb_idx] < cand)
                    ++pb_idx;
                if (pb_idx < 1 && b_seq.data[pb_idx] == cand) {
                    ++cand;
                    continue;
                }
                break;
            }
            b_seq.data[1] = cand;
            ++computed_len;
        } else {                       /* m >= 2 */
            /* a(m) = 4*a(m-2)+b(m-1)+b(m-2) */
            a_seq.data[m] = (seq_t)4 * a_seq.data[m - 2] +
                            b_seq.data[m - 1] + b_seq.data[m - 2];

            /* Find b(m) : smallest integer > b(m-1) not in a[0..m]∪b[0..m-1] */
            seq_t cand = b_seq.data[m - 1] + (seq_t)1;
            for (;;) {
                while (pa_idx <= m && a_seq.data[pa_idx] < cand)
                    ++pa_idx;
                if (pa_idx <= m && a_seq.data[pa_idx] == cand) {
                    ++cand;
                    continue;
                }
                while (pb_idx < m && b_seq.data[pb_idx] < cand)
                    ++pb_idx;
                if (pb_idx < m && b_seq.data[pb_idx] == cand) {
                    ++cand;
                    continue;
                }
                break;
            }
            b_seq.data[m] = cand;
            ++computed_len;
        }
    }

    return a_seq.data[n];
}
