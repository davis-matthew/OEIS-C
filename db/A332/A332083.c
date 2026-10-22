/*
 * A332083  Three-column table read by rows giving triples of integers with x,
 *          y, z != 0 and y != 1 such that y^2 - y - x*z = 0, sorted by the
 *          absolute value of y with the negatives first, then by x in
 *          ascending order.
 * https://oeis.org/A332083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 840, i16 n <= 4368, i32 n <= 4368, i64 n <= 4368,
 *            i128 n <= 4368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332083: a(n) is the nth entry in the table read by rows.
   Table: triples (x, y, z) with x*y*z != 0, y != 1, satisfying
   y^2 - y - x*z = 0.  The rows are ordered by |y| (negative first),
   then by x ascending.  The sequence is the concatenation of each
   triple's elements (x first, then y, then z).                             */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* helper: compare two uint64_t values for qsort                */
static int cmp_u64(const void *a, const void *b)
{
    const uint64_t *va = (const uint64_t *)a;
    const uint64_t *vb = (const uint64_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* helper: generate all positive divisors of n, sorted ascending.
   *divs receives the allocated array, *cnt receives its length.      */
static void get_positive_divisors(uint64_t n, uint64_t **divs, size_t *cnt)
{
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = 0;
    uint64_t t = n;

    /* factor 2 */
    int e = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        ++e;
    }
    if (e) {
        primes[pcnt] = 2;
        exps[pcnt++] = e;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p) continue;
        e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt++] = e;
    }
    if (t > 1) {
        primes[pcnt] = t;
        exps[pcnt++] = 1;
    }

    /* estimate number of divisors */
    size_t maxcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        maxcnt *= (exps[i] + 1);

    uint64_t *arr = (uint64_t *)malloc(maxcnt * sizeof(uint64_t));
    size_t sz = 1;
    arr[0] = 1;

    /* generate divisors iteratively */
    for (size_t i = 0; i < pcnt; ++i) {
        size_t cur_sz = sz;
        uint64_t pp = 1;
        for (int k = 0; k < exps[i]; ++k) {
            pp *= primes[i];
            for (size_t j = 0; j < cur_sz; ++j)
                arr[sz++] = arr[j] * pp;
        }
    }

    /* sort */
    qsort(arr, sz, sizeof(uint64_t), cmp_u64);

    *divs = arr;
    *cnt  = sz;
}

/* ------------------------------------------------------------ */
/* main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for offset 1 */

    idx_t target = n - 1;                /* zero‑based index in full sequence */
    idx_t cur = 0;                       /* running index of already seen numbers */

    for (idx_t m = 1; ; ++m) {
        /* ----- y = -m ----- */
        uint64_t N = (uint64_t)m * (m + 1);   /* N = y^2 - y for y = -m */

        uint64_t *posDivs;
        size_t tau;                         /* number of positive divisors of N */
        get_positive_divisors(N, &posDivs, &tau);

        idx_t row_entries = (idx_t)3 * (idx_t)2 * (idx_t)tau;
        if (cur + row_entries > target) {
            idx_t offset_seq = target - cur;            /* 0‑based inside row */
            idx_t row_offset = offset_seq / 3;          /* which x in the row  */
            int part = offset_seq % 3;                  /* 0 → x, 1 → y, 2 → z */

            seq_t y_val = -(seq_t)m;
            seq_t x_val;
            if (row_offset < tau)
                x_val = -(seq_t)posDivs[tau - 1 - row_offset];
            else
                x_val = (seq_t)posDivs[row_offset - tau];
            seq_t z_val = (seq_t)((long long)N / (long long)x_val);

            free(posDivs);
            if (part == 0) return x_val;
            if (part == 1) return y_val;
            return z_val;
        }
        cur += row_entries;
        free(posDivs);

        /* ----- y = +m (skip m = 1 because y must not be 1) ----- */
        if (m >= 2) {
            uint64_t Np = (uint64_t)m * (m - 1);   /* N = y^2 - y for y = +m */

            get_positive_divisors(Np, &posDivs, &tau);
            row_entries = (idx_t)3 * (idx_t)2 * (idx_t)tau;
            if (cur + row_entries > target) {
                idx_t offset_seq = target - cur;
                idx_t row_offset = offset_seq / 3;
                int part = offset_seq % 3;

                seq_t y_val = (seq_t)m;
                seq_t x_val;
                if (row_offset < tau)
                    x_val = -(seq_t)posDivs[tau - 1 - row_offset];
                else
                    x_val = (seq_t)posDivs[row_offset - tau];
                seq_t z_val = (seq_t)((long long)Np / (long long)x_val);

                free(posDivs);
                if (part == 0) return x_val;
                if (part == 1) return y_val;
                return z_val;
            }
            cur += row_entries;
            free(posDivs);
        }
    }
}
