/*
 * A366433  G.f. A(x) satisfies A(x) = 1 + x * ((1 - x) / A(x))^(3/2).
 * https://oeis.org/A366433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the algebraic equation
   U² (1+U)³ = x² (1−x)³  where  U = A−1  and  A(0)=1.
   This yields for n≥2
        2·a(n) = R(n+1) − T(n) − B(n),
   where  R(n) = [xⁿ]x²(1−x)³  (R(2)=1,R(3)=−3,R(4)=3,R(5)=−1, else 0),
   T(n) = Σ_{i=2}^{n−2} a(i)·a(n−i),
   B(n) = Σ_{k=2}^{n−1} Bk·C_{n−k},
   Bk = Σ_{i=1}^{k−1} a(i)·a(k−i),
   Ck = 3·a(k) + 3·Bk + Σ_{i=1}^{k−1} a(i)·Bk−i.
   The code memoises the sequence for any order of requests. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_seq = NULL;   /* stored a(n) values */
static seq_t *b_arr = NULL;   /* Bk values */
static seq_t *c_arr = NULL;   /* Ck values */
static idx_t  cap    = 0;     /* allocated capacity */
static idx_t  max_n  = 0;     /* highest n computed */

/* Ensure arrays of at least n+3 capacity */
static void ensure_capacity(idx_t n)
{
    if (n+3 <= cap) return;
    idx_t new_cap = n + 6; /* a bit extra to avoid frequent reallocs */
    a_seq  = (seq_t *)realloc(a_seq,  new_cap * sizeof(seq_t));
    b_arr  = (seq_t *)realloc(b_arr,  new_cap * sizeof(seq_t));
    c_arr  = (seq_t *)realloc(c_arr,  new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Initialise arrays for the first element */
static void init(void)
{
    if (cap == 0) {
        ensure_capacity(2);
        a_seq[0] = (seq_t)1;          /* a(0) = 1 */
        a_seq[1] = (seq_t)1;          /* a(1) = 1 */
        b_arr[0] = b_arr[1] = (seq_t)0;
        b_arr[2] = a_seq[1] * a_seq[1];   /* B2 = a1² */
        c_arr[0] = (seq_t)1;          /* C0 = 1 */
        /* C1 = 3*a1 + 3*B1 + 0 = 3 */
        c_arr[1] = (seq_t)3;
        max_n = 1;
    }
}

/* RHS of the equation for index n */
static seq_t rhs(idx_t n)
{
    switch (n) {
        case 2: return (seq_t)1;
        case 3: return (seq_t)-3;
        case 4: return (seq_t)3;
        case 5: return (seq_t)-1;
        default: return (seq_t)0;
    }
}

/* Compute a(n) up to requested index */
static void compute_up_to(idx_t n)
{
    if (n <= max_n) return;
    init();
    /* Extend arrays */
    ensure_capacity(n);
    /* Compute from current max+1 up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* cur corresponds to computing a(cur) using n = cur+1 */
        idx_t N = cur + 1;

        /* Sum Bk * C_{N - k} for k=2..cur */
        seq_t sum_bc = (seq_t)0;
        for (idx_t k = 2; k <= cur; ++k) {
            sum_bc += b_arr[k] * c_arr[N - k];
        }

        /* Sum a(i)*a(N - i) for i=2..cur-1 */
        seq_t sum_t = (seq_t)0;
        for (idx_t i = 2; i <= cur - 1; ++i) {
            sum_t += a_seq[i] * a_seq[N - i];
        }

        /* Compute a(cur) */
        seq_t val = (rhs(N) - sum_t - sum_bc) / (seq_t)2;
        a_seq[cur] = val;

        /* Compute B_{cur+1} = Σ_{i=1}^{cur} a(i)*a(cur+1-i) */
        seq_t new_b = (seq_t)0;
        for (idx_t i = 1; i <= cur; ++i) {
            new_b += a_seq[i] * a_seq[cur + 1 - i];
        }
        b_arr[cur + 1] = new_b;

        /* Compute D_{cur} = Σ_{i=1}^{cur-1} a(i)*B_{cur-i} */
        seq_t d_cur = (seq_t)0;
        for (idx_t i = 1; i <= cur - 1; ++i) {
            d_cur += a_seq[i] * b_arr[cur - i];
        }

        /* Compute C_cur = 3*a(cur) + 3*B_{cur} + D_{cur} */
        c_arr[cur] = (seq_t)3 * val + (seq_t)3 * b_arr[cur] + d_cur;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }
    compute_up_to(n);
    return a_seq[n];
}
