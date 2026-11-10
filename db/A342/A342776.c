/*
 * A342776  Positions of words in A342753 in which #0's < #1's.
 * https://oeis.org/A342776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k with #1(w(k)) > #0(w(k)), where w(k) is the k-th
 *            word of A342753: w(k) = (k mod 3 == 0 ? "1" : "0") followed by
 *            w(c(k)), with c(k) = (k-1)/3 if 3|k, else (k-1) - (k-1)/3, and
 *            w(0) empty. Computed by scanning words
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int *A342776_diff = NULL;      /* diff[k] = #1 - #0 in w(k) */
static size_t A342776_dcap = 0, A342776_dsize = 0;
static idx_t *A342776_found = NULL;
static size_t A342776_fcap = 0, A342776_fsize = 0;

static void A342776_extend(void)
{
    if (A342776_diff == NULL) {
        A342776_dcap = 1024;
        A342776_diff = (int *)malloc(A342776_dcap * sizeof(int));
        A342776_fcap = 256;
        A342776_found = (idx_t *)malloc(A342776_fcap * sizeof(idx_t));
        if (!A342776_diff || !A342776_found) exit(EXIT_FAILURE);
        A342776_diff[0] = 0;
        A342776_dsize = 1;
    }
    if (A342776_dsize >= A342776_dcap) {
        A342776_dcap *= 2;
        int *t = (int *)realloc(A342776_diff, A342776_dcap * sizeof(int));
        if (!t) exit(EXIT_FAILURE);
        A342776_diff = t;
    }
    idx_t k = (idx_t)A342776_dsize;
    idx_t a = k - 1;
    idx_t tp = a / 3;
    idx_t child = (k % 3 == 0) ? tp : (a - tp);
    int d = ((k % 3 == 0) ? 1 : -1) + A342776_diff[child];
    A342776_diff[k] = d;
    A342776_dsize++;
    if (d > 0) {
        if (A342776_fsize >= A342776_fcap) {
            A342776_fcap *= 2;
            idx_t *t = (idx_t *)realloc(A342776_found, A342776_fcap * sizeof(idx_t));
            if (!t) exit(EXIT_FAILURE);
            A342776_found = t;
        }
        A342776_found[A342776_fsize++] = k;
    }
}

seq_t a(idx_t n)
{
    while (A342776_fsize < (size_t)n)
        A342776_extend();
    return (seq_t)A342776_found[n - 1];
}
