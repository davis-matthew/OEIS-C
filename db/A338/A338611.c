/*
 * A338611  Numbers all of whose divisors, excluding the divisor 1, are evil
 *          numbers (A001969) with a record number of divisors.
 * https://oeis.org/A338611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the n-th record‑breaking number * where all divisors >1
 *            are evil numbers. * The values are produced by enumerating * m =
 *            3^a · 5^b · 17^c · 257^d · 65537^e, * with a∈{0,1,2,3} and
 *            b,c,d,e∈{0,1}; * then selecting those that set a new record for
 *            * d(m) = (a+1)·2^{b+c+d+e}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct {
    seq_t val;   /* the number itself        */
    unsigned d;  /* divisor count (d(m))     */
} candidate;

/* Generate all 64 candidates of the form above */
static void generate_candidates(candidate* arr, size_t* count)
{
    const seq_t primes[4] = {5,17,257,65537};
    const seq_t pow3[4]   = {1,3,9,27};   /* 3^0 … 3^3   */
    size_t idx = 0;
    for (unsigned a = 0; a < 4; ++a) {
        for (unsigned mask = 0; mask < 16; ++mask) {   /* 4 bits for the other primes */
            seq_t v = pow3[a];
            unsigned d = a + 1;                        /* d factor from 3^a   */
            for (unsigned j = 0; j < 4; ++j) {
                if (mask & (1u << j)) {
                    v *= primes[j];
                    d *= 2;                           /* each selected prime contributes a factor 2 */
                }
            }
            arr[idx].val = v;
            arr[idx].d   = d;
            ++idx;
        }
    }
    *count = idx;
}

/* Comparison function for qsort on candidates */
static int cmp_candidate(const void *p1, const void *p2)
{
    const candidate *a = (const candidate*)p1;
    const candidate *b = (const candidate*)p2;
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    return 0;
}

/* Compute the record numbers and store them in out[] */
static void compute_records(seq_t* out, size_t* rec_cnt)
{
    candidate cand[64];
    size_t total;
    generate_candidates(cand, &total);
    qsort(cand, total, sizeof(candidate), cmp_candidate);

    unsigned max_d = 0;
    size_t cnt = 0;
    for (size_t i = 0; i < total; ++i) {
        if (cand[i].d > max_d) {
            max_d = cand[i].d;
            out[cnt++] = cand[i].val;
        }
    }
    *rec_cnt = cnt;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t records[64];
    static size_t rec_count = 0;

    if (!initialized) {
        compute_records(records, &rec_count);
        initialized = 1;
    }
    if (n == 0 || (idx_t)n > rec_count)
        return 0;
    return records[(idx_t)n - 1];
}
