/*
 * A383721  a(n) is the number of distinct rectangles with integer area that
 *          can be inscribed in a cube with edge length 4*n, as shown in the
 *          linked figure "Cube with inscribed rectangle".
 * https://oeis.org/A383721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x, y) with 1 ≤ x ≤ 4*n-1 to
 *            x² + 8*n² = y². Equivalent to counting factor pairs a·b = 8*n²
 *            with a < b, a≡b (mod 2), and b‑a ≤ 8*n‑2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* integer square root of n (floor) – works for n ≤ 2^64-1 */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 65536;  /* sqrt(2^64-1) < 65536; 8*n² ≤ 8·10⁸ for n≤10⁴ */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* factorisation of a positive integer x into primes <= sqrt(x).
   primes and exponents are written into the supplied arrays (of size ≥ 32). 
   Returns the number of distinct prime factors. */
static size_t factor_uint64(uint64_t x, uint64_t *primes, unsigned char *exps)
{
    size_t cnt = 0;
    uint64_t tmp = x;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned char e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* recursive enumeration of divisors of M (factored as primes^exps)
   Count those d ≤ sqrtM that satisfy the parity and difference constraints. */
static void dfs_count(const uint64_t *pr, const unsigned char *ex,
                      size_t num, size_t idx, uint64_t cur, uint64_t M,
                      uint64_t sqrtM, uint64_t n, uint64_t *res)
{
    if (idx == num) {
        if (cur > sqrtM) return;                /* only need d ≤ √M */
        uint64_t other = M / cur;
        if ((cur & 1) != (other & 1)) return;   /* parity must match */
        if (other - cur <= 8ULL * n - 2ULL) ++(*res);
        return;
    }
    uint64_t cur_val = cur;
    for (unsigned char e = 0; e <= ex[idx]; ++e) {
        dfs_count(pr, ex, num, idx + 1, cur_val, M, sqrtM, n, res);
        cur_val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safely handle n=0, though offset is 1 */

    /* factor n */
    uint64_t p_n[32];
    unsigned char e_n[32];
    size_t cnt_n = factor_uint64((uint64_t)n, p_n, e_n);

    /* build factorisation of M = 8 * n^2 */
    uint64_t p_M[32];
    unsigned char e_M[32];
    size_t cnt_M = 0;

    bool two_present = false;
    size_t idx_two = 0;

    for (size_t i = 0; i < cnt_n; ++i) {
        p_M[cnt_M] = p_n[i];
        e_M[cnt_M] = 2 * e_n[i];         /* exponent in n^2 */
        if (p_n[i] == 2) {
            two_present = true;
            idx_two = cnt_M;
        }
        ++cnt_M;
    }

    if (two_present) e_M[idx_two] += 3;  /* multiply by 8 = 2^3 */
    else {                               /* add new prime factor 2 */
        p_M[cnt_M] = 2;
        e_M[cnt_M] = 3;
        ++cnt_M;
    }

    /* compute M and its square root */
    uint64_t M      = 8ULL * n * n;
    uint64_t sqrtM  = isqrt_u64(M);

    /* count divisors satisfying conditions */
    uint64_t count = 0;
    dfs_count(p_M, e_M, cnt_M, 0, 1ULL, M, sqrtM, n, &count);

    return (seq_t)count;
}
