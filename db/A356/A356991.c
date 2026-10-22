/*
 * A356991  a(n) = b(n) + b(n - b(n)) for n >= 2, where b(n) = A356988(n).
 * https://oeis.org/A356991
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) + b(n - b(n)) with b(n) = A356988(n). Using the two
 *            convenient formulae from the OEIS entry: (1) a(4*F(k‑1) + j) =
 *            L(k) for 0 ≤ j ≤ F(k‑4) (F(-1)=1) (2) a(F(k+2) + j) = L(k) + j
 *            for 0 ≤ j ≤ L(k‑1) where F is Fibonacci and L is Lucas. This
 *            function returns a(n) for all n ≥ 2
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *fib = NULL;          /* Fibonacci numbers */
static size_t fib_len = 0;         /* number of Fibonacci numbers stored */
static seq_t *lucas = NULL;        /* Lucas numbers */
static size_t lucas_len = 0;       /* number of Lucas numbers stored */

/* Ensure fib[0 .. idx] exist. */
static void ensure_fib_upto(size_t idx)
{
    if (fib == NULL) {
        fib_len = 2;
        fib = (seq_t *)malloc(sizeof(seq_t) * fib_len);
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
    }
    if (fib_len > idx) return;
    size_t old = fib_len;
    fib_len = idx + 1;
    fib = (seq_t *)realloc(fib, sizeof(seq_t) * fib_len);
    for (size_t i = old; i <= idx; ++i)
        fib[i] = fib[i-1] + fib[i-2];
}

/* Ensure lucas[0 .. idx] exist. */
static void ensure_lucas_upto(size_t idx)
{
    if (lucas == NULL) {
        lucas_len = 2;
        lucas = (seq_t *)malloc(sizeof(seq_t) * lucas_len);
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
    }
    if (lucas_len > idx) return;
    size_t old = lucas_len;
    lucas_len = idx + 1;
    lucas = (seq_t *)realloc(lucas, sizeof(seq_t) * lucas_len);
    for (size_t i = old; i <= idx; ++i)
        lucas[i] = lucas[i-1] + lucas[i-2];
}

seq_t a(idx_t n)
{
    /* Base offsets as given in the OEIS entry. */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n < 2) return (seq_t)0;   /* undefined but never requested */

    seq_t n_seq = (seq_t)n;

    for (size_t k = 3; ; ++k) {
        /* ------------- Formula (2) : increasing part ------------- */
        size_t start_idx = k + 2;                 /* F(k+2) */
        ensure_fib_upto(start_idx);
        seq_t start  = fib[start_idx];
        ensure_lucas_upto(k);
        seq_t Lk      = lucas[k];
        seq_t Lk_minus1 = lucas[k-1];
        seq_t end = start + Lk_minus1;

        if (start <= n_seq && n_seq <= end) {
            seq_t offset = n_seq - start;
            return Lk + offset;
        }

        /* ------------- Formula (1) : plateau part ------------- */
        seq_t f_k_minus_1 = fib[k-1];            /* F(k-1) */
        seq_t start_plateau = (seq_t)4 * f_k_minus_1;

        seq_t extra_j;                           /* number of j's in plateau */
        if (k >= 4) {                           /* F(k-4) exists */
            ensure_fib_upto(k-4);
            extra_j = fib[k-4];
        } else if (k == 3) {                    /* F(-1) = 1 */
            extra_j = (seq_t)1;
        } else {                                /* never reached */
            extra_j = (seq_t)0;
        }

        seq_t end_plateau = start_plateau + extra_j;
        if (start_plateau <= n_seq && n_seq <= end_plateau) {
            return Lk; /* constant value of the plateau */
        }
        /* if n is larger than the current end, the loop continues */
    }
}
