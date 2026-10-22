/*
 * A340527  Triangle read by rows: T(n,k) = A024916(n-k+1)*A000041(k-1), 1 <=
 *          k <= n.
 * https://oeis.org/A340527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = A024916(n-k+1)*A000041(k-1)
   a(n) is the sequence read by rows.  The function a() returns the
   n'th term of OEIS A340527 for n ≥ 1. */

#include <stdlib.h>

static seq_t *sigma_cum_ptr = NULL;   /* A024916 values (cumulative sigma) */
static seq_t *partition_ptr = NULL;   /* A000041 values (partition numbers) */
static size_t cache_N = 0;            /* maximum n that has been pre‑computed */

/* Pre‑compute A024916[1..n] and A000041[0..n] */
static void ensure_precomputed(idx_t n)
{
    size_t N = (size_t)n;
    if (sigma_cum_ptr && N <= cache_N) return;   /* already computed */

    size_t size = N + 1;      /* indices 0..N */

    /* Allocate storage.  Use C++ new[] if compiling as C++,
       otherwise use malloc/calloc. */
#ifdef __cplusplus
    seq_t *new_sigma = new seq_t[size];
    seq_t *new_part  = new seq_t[size];
    seq_t *sigma_temp = new seq_t[size];
#else
    seq_t *new_sigma = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *new_part  = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *sigma_temp = (seq_t *)calloc(size, sizeof(seq_t));
#endif
    if (!new_sigma || !new_part) return;   /* allocation failed */

    /* Compute sigma_temp[m] = σ(m) for 1 ≤ m ≤ N.  */
    for (size_t d = 1; d <= N; ++d) {
        seq_t d_seq = (seq_t)d;
        for (size_t m = d; m <= N; m += d) {
            sigma_temp[m] += d_seq;
        }
    }

    /* Compute cumulative sums A024916: σ_1 + σ_2 + … + σ_m. */
    seq_t acc = (seq_t)0;
    for (size_t i = 1; i <= N; ++i) {
        acc += sigma_temp[i];
        new_sigma[i] = acc;
    }

    /* Free sigma_temp. */
#ifdef __cplusplus
    delete[] sigma_temp;
#else
    free(sigma_temp);
#endif

    /* Compute partition numbers A000041 using the pentagonal
       theorem.  Partition(0) = 1. */
    new_part[0] = (seq_t)1;
    for (size_t nn = 1; nn <= N; ++nn) {
        seq_t sum = (seq_t)0;
        for (int k = 1; ; ++k) {
            size_t pent1 = (size_t)k * (3 * k - 1) / 2;
            if (pent1 > nn) break;
            seq_t term = new_part[nn - pent1];
            if (k & 1) sum += term; else sum -= term;
            size_t pent2 = (size_t)k * (3 * k + 1) / 2;
            if (pent2 > nn) continue;
            seq_t term2 = new_part[nn - pent2];
            if (k & 1) sum += term2; else sum -= term2;
        }
        new_part[nn] = sum;
    }

    /* Replace old cache (freeing it first). */
#ifdef __cplusplus
    delete[] sigma_cum_ptr;
    delete[] partition_ptr;
#else
    free(sigma_cum_ptr);
    free(partition_ptr);
#endif

    sigma_cum_ptr = new_sigma;
    partition_ptr = new_part;
    cache_N = N;
}

/* Return the n-th term of A340527 (1‑based). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* invalid, but not used */

    ensure_precomputed(n);

    /* Find the row r such that T(r) = r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((unsigned long long)r * (r + 1) / 2 < n) ++r;

    idx_t start = r * (r - 1) / 2 + 1;     /* first index in row r */
    idx_t k     = n - start + 1;           /* column in that row   */
    idx_t m     = r - k + 1;               /* index for A024916     */

    /* T(r,k) = A024916(m) * A000041(k-1). */
    seq_t result = sigma_cum_ptr[m] * partition_ptr[k - 1];
    return result;
}
