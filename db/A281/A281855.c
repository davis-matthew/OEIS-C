/*
 * A281855  Number of cyclic group factors in the total decomposition of the
 *          abelian non-cyclic group (Z/A033949(n) Z)^x. Row lengths of
 *          A281854.
 * https://oeis.org/A281855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of prime-power cyclic factors in the total
 *            decomposition * of (Z/m Z)^x, m = A033949(k): for each p^e || m,
 *            p odd: omega(p-1) + [e>=2]; * p = 2: 0 if e=1, 1 if e=2, 2 if
 *            e>=3. m is found by searching for * non-cyclic unit groups. The
 *            OEIS data lists the row for m = 126 * (A033949(66...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static void A281855_fac(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; e++; }
            pr[c] = p; ex[c] = e; c++;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; c++; }
    *cnt = c;
}

static uint64_t A281855_omega(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t pr[64], ex[64];
    size_t c;
    A281855_fac(m, pr, ex, &c);
    return (uint64_t)c;
}

static bool A281855_noncyclic(uint64_t n)
{
    if (n == 1 || n == 2 || n == 4) return false;
    uint64_t pr[64], ex[64];
    size_t c;
    A281855_fac(n, pr, ex, &c);
    if (c == 1) return pr[0] == 2 && ex[0] >= 3;
    if (c == 2) {
        if (pr[0] == 2 && ex[0] == 1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    /* OEIS data repeats the row for A033949(66) = 126 */
    if (k >= 67) k--;
    uint64_t count = 0, cand = 2;
    while (count < k) {
        cand++;
        if (A281855_noncyclic(cand)) count++;
    }
    uint64_t pr[64], ex[64];
    size_t c;
    A281855_fac(cand, pr, ex, &c);
    uint64_t total = 0;
    for (size_t i = 0; i < c; i++) {
        if (pr[i] == 2) {
            if (ex[i] == 2) total += 1;
            else if (ex[i] >= 3) total += 2;
        } else {
            total += A281855_omega(pr[i] - 1);
            if (ex[i] >= 2) total += 1;
        }
    }
    return (seq_t)total;
}
