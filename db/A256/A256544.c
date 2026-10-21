/*
 * A256544  Number of ways to write n as the sum of three unordered elements
 *          of the set {floor(T(x)/3): x = 1,2,3,...}, where T(x) denotes the
 *          triangular number x*(x+1)/2.
 * https://oeis.org/A256544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A256544:
    a(n) = the number of unordered triples (i,j,k) with
    i ≤ j ≤ k, i,j,k ∈ {⌊T(x)/3⌋ : x = 1,2,3,…}
    and i + j + k = n.                           */

#include <stdlib.h>

static idx_t _max_computed = 0;      /* largest n already processed   */
static seq_t * _counts = NULL;      /* a(0)…a(_max_computed)         */
static idx_t * _vals   = NULL;      /* distinct ⌊T(x)/3⌋ values       */
static size_t  _val_cnt = 0;        /* number of distinct values      */

/*  Recompute all a(n) up to max_n.  */
static void _recompute(idx_t max_n) {
    /*  Gather distinct ⌊T(x)/3⌋ values ≤ max_n.  */
    idx_t   k   = 0;
    size_t  cap = 512;              /* big enough for n up to about 2·10⁴   */
    idx_t * buf      = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t  m        = 0;
    seq_t   last_val = (seq_t)-1;    /* sentinel – different from any real val */

    for (;;) {
        seq_t val = ((seq_t)k * (k + 1)) / 6;   /* ⌊T(k)/3⌋  */
        if (val > max_n) break;
        if (m == 0 || val != last_val) {
            if (m == cap) {                 /* grow buffer */
                cap <<= 1;
                buf = (idx_t *)realloc(buf, cap * sizeof(idx_t));
            }
            buf[m++] = (idx_t)val;
            last_val = val;
        }
        ++k;
    }

    /*  Free any previous data and allocate fresh arrays. */
    if (_counts) free(_counts);
    if (_vals)   free(_vals);

    _counts = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
    _vals   = buf;
    _val_cnt = m;
    _max_computed = max_n;

    /*  Count unordered triples a ≤ b ≤ c.          */
    for (size_t i = 0; i < m; ++i) {
        idx_t a = buf[i];
        for (size_t j = i; j < m; ++j) {
            idx_t b = buf[j];
            for (size_t l = j; l < m; ++l) {
                idx_t c  = buf[l];
                idx_t sum = a + b + c;
                if (sum <= max_n)
                    _counts[sum] += 1;   /* each unordered triple counted once */
            }
        }
    }
}

seq_t a(idx_t n) {
    /*  Initialise on first use or extend if necessary. */
    if (_counts == NULL || n > _max_computed)
        _recompute(n);
    return _counts[n];
}
