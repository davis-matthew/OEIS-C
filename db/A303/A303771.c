/*
 * A303771  Divisor-or-multiple permutation of natural numbers, "Fermi-Dirac
 *          piano played with May code": a(n) = A052330(A303767(n)).
 * https://oeis.org/A303771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052330(A303767(n)); A303767 is the May code (a(0)=0;
 *            a(2^k)=2^k+a(2^k-1); a(2^k+r)=2^k+a(r-1) for 0<r<2^k).
 *            A052330(m) is the product of the Fermi-Dirac primes b_i (the
 *            numbers p^(2^k), in increasing order) over the set bits i of m
 * Range:     u8 n <= 15, u16 n <= 122, u32 n <= 2046, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 9619 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool fd_is_prime(uint64_t m)
{
    if (m < 2) return false;
    for (uint64_t d = 2; d * d <= m; ++d)
        if (m % d == 0) return false;
    return true;
}

/* m is a Fermi-Dirac prime: p^(2^k) with p prime */
static bool fd_is_fdprime(uint64_t m)
{
    for (;;) {
        if (fd_is_prime(m)) return true;
        uint64_t s = 0;
        while (s * s < m) ++s;
        if (s * s != m || s < 2) return false;
        m = s;
    }
}

static uint64_t fd_b[64];
static int fd_cnt = 0;
static uint64_t fd_next = 2;

static void fd_fill(int k)
{
    while (fd_cnt <= k) {
        while (!fd_is_fdprime(fd_next)) ++fd_next;
        fd_b[fd_cnt++] = fd_next;
        ++fd_next;
    }
}

static idx_t may_code(idx_t n)
{
    idx_t res = 0;
    while (n != 0) {
        idx_t p = 1;
        while (p <= n / 2) p <<= 1;
        if (p == n) {
            res += n;
            n -= 1;
        } else {
            res += p;
            n = n - p - 1;
            /* a(0)=0 handled by loop ending; but r-1 >= 0 always since r>=1 */
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = may_code(n);
    fd_fill(63);
    seq_t res = 1;
    for (int i = 0; i < 64 && (m >> i) != 0; ++i) {
        if ((m >> i) & 1)
            res *= (seq_t)fd_b[i];
    }
    return res;
}
