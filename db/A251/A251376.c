/*
 * A251376  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock summing
 *          to 5 6 or 7.
 * https://oeis.org/A251376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Formula: a(n) = number of (n+1)×4 arrays with entries 0..3
 *              such that every 2×2 subblock sums to 5, 6, or 7.
 * Computation: DP on row transitions for small n, then a linear
 *              recurrence of order 52 derived from the first 104 terms.
 */

/* Constants */
#define STATES 256
#define K 52
#define MAX_SMALL_N 100

/* Transition tables */
static unsigned char next_cnt[STATES];
static unsigned char next_list[STATES][STATES];
static bool transition_ready = false;

/* Small DP cache for direct computation */
static seq_t small_terms[MAX_SMALL_N];
static bool small_ready = false;

/* Recurrence data */
static seq_t init_rec[K];
static seq_t rec_coeff[K];
static bool recurrence_ready = false;

/* --------------------------------------------------------------------- */
/* Build transition lists:  from row i to row j is allowed when every
   2×2 subblock (rows i and j) summed over 3 adjacent columns is 5,6 or 7. */
static void build_transition(void)
{
    if (transition_ready) return;
    for (int i = 0; i < STATES; ++i) {
        unsigned char a[4];
        unsigned val = (unsigned)i;
        a[0] = val & 3U;      val >>= 2;
        a[1] = val & 3U;      val >>= 2;
        a[2] = val & 3U;      val >>= 2;
        a[3] = val & 3U;
        unsigned char *dest = next_list[i];
        unsigned cnt = 0;
        for (int j = 0; j < STATES; ++j) {
            unsigned char b[4];
            unsigned valj = (unsigned)j;
            b[0] = valj & 3U;      valj >>= 2;
            b[1] = valj & 3U;      valj >>= 2;
            b[2] = valj & 3U;      valj >>= 2;
            b[3] = valj & 3U;
            bool ok = true;
            for (int col = 0; col < 3; ++col) {
                int sum = a[col] + a[col+1] + b[col] + b[col+1];
                if (sum != 5 && sum != 6 && sum != 7) { ok = false; break; }
            }
            if (ok) dest[cnt++] = (unsigned char)j;
        }
        next_cnt[i] = cnt;
    }
    transition_ready = true;
}

/* Generate terms 1..upto (inclusive).  The array out must have
   space for upto elements.  Assumes transition table ready. */
static void generate_terms(idx_t upto, seq_t *out)
{
    if (!transition_ready) build_transition();
    static seq_t cur[STATES], nxt[STATES];
    for (size_t i = 0; i < STATES; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 1; step <= upto; ++step) {
        for (size_t i = 0; i < STATES; ++i) nxt[i] = (seq_t)0;
        for (size_t i = 0; i < STATES; ++i) {
            seq_t vi = cur[i];
            if (vi == 0) continue;
            unsigned cnt = next_cnt[i];
            unsigned char const *list = next_list[i];
            for (unsigned k = 0; k < cnt; ++k)
                nxt[list[k]] += vi;
        }
        seq_t s = 0;
        for (size_t i = 0; i < STATES; ++i) s += nxt[i];
        out[step-1] = s;
        for (size_t i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }
}

/* Small DP query for n <= MAX_SMALL_N.  Results cached on first use. */
static seq_t term_small(idx_t n)
{
    if (!small_ready) {
        generate_terms(MAX_SMALL_N, small_terms);
        small_ready = true;
    }
    return small_terms[n-1];
}

/* --------------------------------------------------------------------- */
/* Build recurrence coefficients of order K from the first 2K generated
   terms.  The method uses a Bareiss (fraction‑free) Gaussian elimination
   on the augmented matrix.  The matrix is constructed from the
   recurrence equation:
       a[k+i] = sum_{j=0}^{k-1} r[j] * a[k+i-j-1]   (i = 0..k-1) */
static void compute_recurrence(void)
{
    if (recurrence_ready) return;
    build_transition();

    /* Obtain the first 2K terms */
    seq_t terms[2*K];
    generate_terms(2*K, terms);

    /* Fill initial vector */
    for (int i = 0; i < K; ++i) init_rec[i] = terms[i];

    /* Construct augmented matrix A (K × (K+1)) */
    static seq_t A[K][K+1];
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            A[i][j] = terms[K + i - j - 1];
        }
        A[i][K] = terms[K + i];
    }

    /* Bareiss elimination */
    seq_t prev = 1;
    for (int i = 0; i < K; ++i) {
        /* Pivot search */
        if (A[i][i] == 0) {
            int r = -1;
            for (int rr = i+1; rr < K; ++rr) {
                if (A[rr][i] != 0) { r = rr; break; }
            }
            if (r == -1) { /* singular – shouldn't happen */
                for (int t = 0; t < K; ++t) rec_coeff[t] = 0;
                recurrence_ready = true;
                return;
            }
            for (int col = 0; col <= K; ++col) {
                seq_t tmp = A[i][col];
                A[i][col] = A[r][col];
                A[r][col] = tmp;
            }
        }

        seq_t piv = A[i][i];
        for (int j = i+1; j < K; ++j) {
            for (int l = i+1; l <= K; ++l) {
                seq_t num = piv * A[j][l] - A[j][i] * A[i][l];
                /* Division is exact */
                A[j][l] = num / prev;
            }
        }
        prev = piv;
    }

    /* Back‑substitution */
    seq_t x[K];
    for (int i = K-1; i >= 0; --i) {
        seq_t sum = A[i][K];
        for (int j = i+1; j < K; ++j)
            sum -= A[i][j] * x[j];
        /* Division is exact */
        x[i] = sum / A[i][i];
    }

    for (int i = 0; i < K; ++i) rec_coeff[i] = x[i];
    recurrence_ready = true;
}

/* Polynomial multiplication modulo the characteristic polynomial
   defined by rec_coeff.  Input and output arrays have length K. */
static void poly_mul(const seq_t *a, const seq_t *b, seq_t *out,
                     const seq_t *rec)
{
    static seq_t tmp[2*K-1];
    for (int i = 0; i < 2*K-1; ++i) tmp[i] = (seq_t)0;

    for (int i = 0; i < K; ++i) {
        if (a[i] == 0) continue;
        for (int j = 0; j < K; ++j) {
            if (b[j] == 0) continue;
            tmp[i+j] += a[i] * b[j];
        }
    }

    for (int i = 2*K-2; i >= K; --i) {
        if (tmp[i] == 0) continue;
        for (int j = 1; j <= K; ++j)
            tmp[i-j] += tmp[i] * rec[j-1];
        tmp[i] = (seq_t)0;
    }

    for (int i = 0; i < K; ++i) out[i] = tmp[i];
}

/* Compute a(n) for n > MAX_SMALL_N using the recurrence. */
static seq_t linear_term(idx_t n)
{
    static seq_t res[K];
    static seq_t base[K];
    static seq_t temp[K];

    for (int i = 0; i < K; ++i) {
        res[i]  = (seq_t)0;
        base[i] = (seq_t)0;
    }
    res[0] = (seq_t)1;          /* 1 */
    base[1] = (seq_t)1;         /* x */

    idx_t exp = n - 1;          /* zero‑based exponent */
    while (exp) {
        if (exp & 1LL) {
            poly_mul(res, base, temp, rec_coeff);
            for (int i = 0; i < K; ++i) res[i] = temp[i];
        }
        poly_mul(base, base, temp, rec_coeff);
        for (int i = 0; i < K; ++i) base[i] = temp[i];
        exp >>= 1;
    }

    seq_t ans = 0;
    for (int i = 0; i < K; ++i) ans += res[i] * init_rec[i];
    return ans;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    /* Small n: direct DP */
    if (n <= MAX_SMALL_N) return term_small(n);

    /* Large n: use recurrence (build if necessary) */
    if (!recurrence_ready) compute_recurrence();
    return linear_term(n);
}
