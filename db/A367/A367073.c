/*
 * A367073  G.f. satisfies A(x) = 1 + 2*x + x^3*A(x)^3.
 * https://oeis.org/A367073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367073
   Recurrence: a(0)=1, a(1)=2, a(2)=0.
   For n>=3: a(n) = Σ_{i+j+k=n-3} a(i)*a(j)*a(k)
   The above is equivalent to a(n)=Σ_{l=0}^{n-3} B(l)*a(n-3-l),
   where B(l)=Σ_{i+j=l} a(i)*a(j).
   The implementation keeps a and the convolution B in static
   arrays and extends them as needed, enabling arbitrary order of
   calls to a(idx_t n). */

#include <stdlib.h>
#include <stdint.h>

/* external types:
   seq_t   – integer type for sequence values (e.g. uint64_t,
             unsigned __int128 or arbitrary precision class).
   idx_t   – unsigned 64‑bit integer for indices. */

static seq_t *A = NULL;          /* sequence values */
static seq_t *B = NULL;          /* convolution values B(i)=Σ_{p+q=i} A[p]*A[q] */
static size_t a_cap = 0;         /* capacity of A */
static size_t b_cap = 0;         /* capacity of B */
static size_t a_len = 0;         /* number of A elements currently valid
                                 * (next free index) */

/* Ensure A and B have capacity for indices up to N (inclusive),
   and compute all terms up to N.  All values are stored in static
   arrays so that subsequent calls use cached results. */
static void ensure_cache(idx_t N)
{
    size_t needed_a = N + 1;
    size_t needed_b = 2 * N + 1;   /* B indices up to 2N may be needed */

    /* Allocate or grow A */
    if (a_cap == 0) {
        a_cap = 32;
        A = (seq_t *)calloc(a_cap, sizeof(seq_t));
    } else if (needed_a > a_cap) {
        size_t new_cap = a_cap;
        while (new_cap < needed_a) new_cap <<= 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        /* zero the newly allocated part */
        if (new_cap > a_cap)
            memset(A + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Allocate or grow B */
    if (b_cap == 0) {
        b_cap = 64;
        B = (seq_t *)calloc(b_cap, sizeof(seq_t));
    } else if (needed_b > b_cap) {
        size_t new_cap = b_cap;
        while (new_cap < needed_b) new_cap <<= 1;
        B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (new_cap > b_cap)
            memset(B + b_cap, 0, (new_cap - b_cap) * sizeof(seq_t));
        b_cap = new_cap;
    }

    /* If nothing has been computed yet, initialise the seeds */
    if (a_len == 0) {
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        A[2] = (seq_t)0;
        /* initialise B for indices up to 4 (sum of 0..2) */
        for (idx_t i = 0; i <= 2; ++i)
            for (idx_t j = 0; j <= 2; ++j) {
                idx_t idx = i + j;
                B[idx] += A[i] * A[j];
            }
        a_len = 3; /* next free index */
    }

    /* Compute terms from a_len up to N */
    for (idx_t k = a_len; k <= N; ++k) {
        if (k < 3) continue;   /* only needed for k>=3 */

        /* a(k) = Σ_{l=0}^{k-3} B(l)*A(k-3-l) */
        seq_t sum = 0;
        for (idx_t l = 0; l <= k - 3; ++l) {
            sum += B[l] * A[(size_t)(k - 3 - l)];
        }
        A[(size_t)k] = sum;

        /* Update B with the contribution of A(k) */
        for (idx_t i = 0; i < k; ++i) {
            size_t idx = k + i;
            seq_t prod = A[(size_t)k] * A[(size_t)i];
            B[idx] += prod + prod;   /* two ordered pairs (k,i) and (i,k) */
        }
        B[2 * k] += A[(size_t)k] * A[(size_t)k];  /* pair (k,k) */
    }

    a_len = N + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;

    ensure_cache(n);
    return A[(size_t)n];
}
