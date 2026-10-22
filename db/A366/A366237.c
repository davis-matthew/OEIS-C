/*
 * A366237  G.f. A(x) satisfies A(x) = 1 + x + x*(1 + x)^3*A(x)^2.
 * https://oeis.org/A366237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(4*k+1, n-k) * binomial(2*k,k)/(k+1)
 *            Recurrence (derived from A(x)=1+x+x(1+x)^3A(x)^2): a(0)=1,
 *            a(1)=2 For n ≥ 2, a(n) = c[n-1] + 3*c[n-2] + 3*c[n-3] + c[n-4]
 *            where c[k] = ∑_{i=0..k} a(i)*a(k-i) (convolution of a with
 *            itself)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
typedef unsigned __int128 int128_t;
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

#if HAVE_INT128
/* use 128‑bit intermediates when seq_t is 64‑bit wide */
static int use_int128 = -1; /* -1 = undecided, 0 = no, 1 = yes */
#endif

/* compute a(n) */
seq_t a(idx_t n)
{
    static seq_t *a_arr      = NULL;
    static seq_t *c_arr      = NULL;           /* used if !use_int128 */
    static int128_t *c128_arr = NULL;          /* used if use_int128    */
    static idx_t  max_n      = (idx_t)-1;
    static size_t c_size     = 0;
#if HAVE_INT128
    if (use_int128 == -1) {
        use_int128 = (sizeof(seq_t) == 8) ? 1 : 0;
    }
#endif

    /* ---------- initialization or resize ---------- */
    if (max_n == (idx_t)-1) {                /* first call */
        size_t a_need = (size_t)(n + 1);
        size_t c_need = (size_t)(2 * n + 1);

        a_arr = (seq_t*)malloc(a_need * sizeof(seq_t));
        if (!a_arr) abort();

#if HAVE_INT128
        if (use_int128) {
            c128_arr = (int128_t*)calloc(c_need, sizeof(int128_t));
            if (!c128_arr) abort();
        } else
#endif
        {
            c_arr = (seq_t*)calloc(c_need, sizeof(seq_t));
            if (!c_arr) abort();
        }
        c_size = c_need;

        /* initialize first term */
        a_arr[0] = (seq_t)1;
#if HAVE_INT128
        if (use_int128)
            c128_arr[0] = (int128_t)1;
        else
#endif
            c_arr[0]   = (seq_t)1;
    } else if ((size_t)(n + 1) > (size_t)(max_n + 1) ||
               (size_t)(2 * n + 1) > c_size) {
        /* enlarge arrays */
        size_t new_a_size = (size_t)(n + 1);
        if (new_a_size > (size_t)(max_n + 1)) {
            a_arr = (seq_t*)realloc(a_arr, new_a_size * sizeof(seq_t));
            if (!a_arr) abort();
        }
        size_t new_c_size = (size_t)(2 * n + 1);
        if (new_c_size > c_size) {
#if HAVE_INT128
            if (use_int128) {
                c128_arr = (int128_t*)realloc(c128_arr,
                                              new_c_size * sizeof(int128_t));
                if (!c128_arr) abort();
                for (size_t i = c_size; i < new_c_size; ++i)
                    c128_arr[i] = (int128_t)0;
            } else
#endif
            {
                c_arr = (seq_t*)realloc(c_arr,
                                       new_c_size * sizeof(seq_t));
                if (!c_arr) abort();
                for (size_t i = c_size; i < new_c_size; ++i)
                    c_arr[i] = (seq_t)0;
            }
            c_size = new_c_size;
        }
    }

    /* ---------- compute missing terms ---------- */
    for (idx_t k = (max_n == (idx_t)-1) ? 1 : (max_n + 1); k <= n; ++k) {
        /* compute a(k) */
        seq_t ak = (k == 1) ? (seq_t)1 : (seq_t)0;
        if (k >= 1) {
#if HAVE_INT128
            if (use_int128)
                ak += (seq_t)c128_arr[k - 1];
            else
#endif
                ak += c_arr[k - 1];
        }
        if (k >= 2) {
#if HAVE_INT128
            if (use_int128)
                ak += (seq_t)(3 * c128_arr[k - 2]);
            else
#endif
                ak += (seq_t)(3 * c_arr[k - 2]);
        }
        if (k >= 3) {
#if HAVE_INT128
            if (use_int128)
                ak += (seq_t)(3 * c128_arr[k - 3]);
            else
#endif
                ak += (seq_t)(3 * c_arr[k - 3]);
        }
        if (k >= 4) {
#if HAVE_INT128
            if (use_int128)
                ak += (seq_t)c128_arr[k - 4];
            else
#endif
                ak += c_arr[k - 4];
        }
        a_arr[k] = ak;

        /* update convolution c */
        for (idx_t i = 0; i < k; ++i) {
            idx_t m = i + k;
#if HAVE_INT128
            if (use_int128) {
                int128_t prod = (int128_t)a_arr[i] * (int128_t)ak;
                c128_arr[m] += 2 * prod;
            } else
#endif
            {
                seq_t prod = a_arr[i] * ak;
                c_arr[m] += 2 * prod;
            }
        }
        /* pair (k,k) */
        {
            idx_t m = 2 * k;
#if HAVE_INT128
            if (use_int128) {
                int128_t prod = (int128_t)ak * (int128_t)ak;
                c128_arr[m] += prod;
            } else
#endif
            {
                seq_t prod = ak * ak;
                c_arr[m] += prod;
            }
        }
    }

    if (max_n == (idx_t)-1)
        max_n = n;
    else if ((idx_t)n > max_n)
        max_n = n;

    return a_arr[n];
}
