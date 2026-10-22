/*
 * A331392  Sum, over all binary strings w of length n, of the length of the
 *          shortest border of w.
 * https://oeis.org/A331392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊n/2⌋} m * A045690(m) * 2^{n-2m}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A045690(m) = count of binary unbordered words of length m      */
/*   2^{i} is pre‑computed in pow2_arr[i]                            */

#include <stdlib.h>

/* ----------  power of two  ------------------------------------- */
static seq_t *pow2_arr  = NULL;   /* pow2_arr[i] = 2^i */
static size_t pow2_cap  = 0;       /* number of initialized entries */

/* ensure pow2_arr contains powers of two up to exponent "need"   */
static void ensure_pow2(size_t need)
{
    if (pow2_cap == 0) {
        pow2_arr = (seq_t*)malloc(sizeof(seq_t));
        pow2_arr[0] = (seq_t)1;
        pow2_cap   = 1;
    }
    if (pow2_cap > need + 1)
        return;

    size_t new_cap = need + 1;
    pow2_arr = (seq_t*)realloc(pow2_arr, new_cap * sizeof(seq_t));
    for (size_t i = pow2_cap; i < new_cap; ++i)
        pow2_arr[i] = pow2_arr[i - 1] * (seq_t)2;
    pow2_cap = new_cap;
}

/* ----------  unbordered word counts  ---------------------------- */
static seq_t *u_arr     = NULL;   /* u_arr[m] = A045690(m) (unbordered word count) */
static size_t u_cap     = 0;       /* number of initialized entries */

/* ensure u_arr contains A045690(m) up to index "need"           */
static void ensure_u(size_t need)
{
    if (u_cap == 0) {
        u_arr = (seq_t*)malloc(sizeof(seq_t));
        u_arr[0] = (seq_t)0;          /* unused, but safe */
        u_cap   = 1;
    }
    if (u_cap > need + 1)
        return;

    size_t new_cap = need + 1;
    u_arr = (seq_t*)realloc(u_arr, new_cap * sizeof(seq_t));

    for (size_t m = u_cap; m < new_cap; ++m) {
        if (m == 0) {
            u_arr[0] = (seq_t)0;
            continue;
        }
        if (m == 1) {
            u_arr[1] = (seq_t)2;      /* "0" and "1" are unbordered */
            continue;
        }
        /* sum_{k=1}^{⌊m/2⌋} u[k] * 2^{m-2k} */
        seq_t sum = (seq_t)0;
        ensure_pow2(m);               /* need powers up to m */

        size_t lim = m / 2;
        for (size_t k = 1; k <= lim; ++k) {
            seq_t term = u_arr[k] * pow2_arr[m - 2 * k];
            sum += term;
        }
        u_arr[m] = pow2_arr[m] - sum; /* 2^m minus the sum */
    }
    u_cap = new_cap;
}

/* ----------  sequence a(n)  ------------------------------------- */
static seq_t *a_arr     = NULL;   /* a_arr[n] = a(n) */
static size_t a_cap     = 0;       /* number of initialized entries */

/* ensure a_arr contains a(n) up to index "need"                  */
static void ensure_a(size_t need)
{
    if (a_cap == 0) {
        a_arr = (seq_t*)malloc(sizeof(seq_t));
        a_arr[0] = (seq_t)0;
        a_cap   = 1;
    }
    if (a_cap > need + 1)
        return;

    size_t new_cap = need + 1;
    a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));

    for (size_t n = a_cap; n < new_cap; ++n) {
        if (n == 0) {
            a_arr[0] = (seq_t)0;
            continue;
        }
        if (n == 1) {
            a_arr[1] = (seq_t)0;
            continue;
        }
        /* compute a[n] = Σ_{m=1}^{⌊n/2⌋} m * u[m] * 2^{n-2m}      */
        size_t max_m = n / 2;
        seq_t sum = (seq_t)0;
        ensure_u(max_m);
        ensure_pow2(n);   /* need pow2 up to n  */

        for (size_t m = 1; m <= max_m; ++m) {
            seq_t term = (seq_t)m * u_arr[m] * pow2_arr[n - 2 * m];
            sum += term;
        }
        a_arr[n] = sum;
    }
    a_cap = new_cap;
}

/* ----------  public function  ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* value below offset is undefined */

    size_t sz = (size_t)n;
    ensure_pow2(sz);
    ensure_u(sz / 2);
    ensure_a(sz);

    return a_arr[sz];
}
