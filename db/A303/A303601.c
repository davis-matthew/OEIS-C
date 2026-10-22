/*
 * A303601  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + Bell(k) +
 *          Bell(m) with 0 <= a <= b and 0 < k <= m, where Bell(k) denotes the
 *          k-th Bell number A000110(k).
 * https://oeis.org/A303601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1985, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of solutions of
 *   n = T(a)+T(b)+Bell(k)+Bell(m)
 * where T(x)=x*(x+1)/2, 0≤a≤b, 1≤k≤m,
 * and Bell(i) is the i‑th Bell number (Bell(1)=1).
 */

#include <stddef.h>
#include <stdbool.h>

/* maximal index for Bell numbers we compute.
   B(27) ≈ 1.5·10¹⁹, enough for all test values. */
#define MAX_BELL_IDX 27

/* storage for Bell numbers */
static seq_t bell[MAX_BELL_IDX + 1];
static size_t bell_len = 0;
static bool bell_ready = false;

/* multiplicative binomial coefficient C(n,k), result in seq_t */
static seq_t binom_seq(size_t n, size_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (size_t i = 1; i <= k; ++i)
        res = (res * (n - k + i)) / i;
    return res;
}

/* generate Bell numbers up to (and including) the first value > limit.
   If limit > 0 we ensure we compute at least the smallest index whose
   Bell number exceeds limit, otherwise we generate the full table up to
   MAX_BELL_IDX.  This routine may be called repeatedly; it simply extends
   the table if necessary. */
static void ensure_bell_upto(seq_t limit)
{
    if (!bell_ready) {
        bell[0] = (seq_t)1;  /* Bell(0) = 1 */
        bell[1] = (seq_t)1;  /* Bell(1) = 1 */
        bell_len = 2;
        bell_ready = true;
    }

    /* extend table while needed and not exceeding MAX_BELL_IDX */
    while (bell_len <= MAX_BELL_IDX) {
        if (bell[bell_len - 1] > limit)
            break;                         /* table already sufficient   */
        size_t i = bell_len;                 /* compute Bell(i)           */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < i; ++k) {
            seq_t c   = binom_seq(i - 1, k);
            sum += c * bell[k];
        }
        bell[bell_len] = sum;
        ++bell_len;
        if (sum > limit)
            break;                         /* next queries will be covered */
    }
}

seq_t a(idx_t n)
{
    /* cast n once; for seq_t sizes smaller than n this will happen only
       for values that fit in seq_t (test cases rely on this). */
    seq_t N = (seq_t)n;

    /* ensure we have Bell numbers up to N (or above) */
    ensure_bell_upto(N);

    seq_t ans = (seq_t)0;

    /* outer loop over a (triangular numbers) */
    for (idx_t a = 0; ; ++a) {
        seq_t Ta = (seq_t)a * (a + 1U) / 2U;
        if (Ta > N) break;

        /* inner loop over b, with 0 ≤ a ≤ b */
        for (idx_t b = a; ; ++b) {
            seq_t Tb = (seq_t)b * (b + 1U) / 2U;
            seq_t Tsum = Ta + Tb;
            if (Tsum > N) break;

            seq_t R = N - Tsum;
            if (R < (seq_t)2) continue;   /* need at least Bell(1)+Bell(1) = 2 */

            /* count pairs (k,m), 1 ≤ k ≤ m, with Bell(k)+Bell(m)=R */
            seq_t cnt = (seq_t)0;
            for (size_t k = 1; k < bell_len; ++k) {
                if (bell[k] > R) break;
                for (size_t m = k; m < bell_len; ++m) {
                    if (bell[m] > R) break;
                    if (bell[k] + bell[m] == R) ++cnt;
                }
            }
            ans += cnt;
        }
    }
    return ans;
}
