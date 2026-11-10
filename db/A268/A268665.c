/*
 * A268665  Number of primes p==1 (mod 3) for which A261029(2*prime(n)*p) is
 *          4-i if prime(n)==i (mod 3), where i=1 or 2.
 * https://oeis.org/A268665
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{primes p == 1 (mod 3) : A261029(2*prime(n)*p) = 4 -
 *            (prime(n) mod 3)}; computed by search over p <= 4*prime(n)^2+10
 *            using A261029(m) = #{(a,b,x): m=s*Q, Q=a^2+ab+b^2, s=3x+2a+b,
 *            a,b>=0, (a,b)!=(0,0), x>=0}
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A268665_isqrt(uint64_t v)
{
    uint64_t r = (uint64_t)__builtin_sqrt((double)v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

static uint64_t A268665_nthprime(idx_t n)
{
    uint64_t c = 1;
    idx_t k = 0;
    while (k < n) {
        c++;
        bool pr = true;
        for (uint64_t d = 2; d * d <= c; d++)
            if (c % d == 0) { pr = false; break; }
        if (pr) k++;
    }
    return c;
}

/* number of (a,b), a,b>=0 not both 0, a^2+ab+b^2=Q, s=m/Q, s>=2a+b, s==2a+b mod 3 */
static uint64_t A268665_reps(uint64_t Q, uint64_t s)
{
    uint64_t cnt = 0;
    uint64_t amax = A268665_isqrt(Q);
    if (s / 2 < amax) amax = s / 2;
    for (uint64_t a = 0; a <= amax; a++) {
        uint64_t D = 4 * Q - 3 * a * a;
        uint64_t r = A268665_isqrt(D);
        if (r * r != D) continue;
        if (r < a || ((r - a) & 1)) continue;
        uint64_t b = (r - a) / 2;
        if (a == 0 && b == 0) continue;
        if (2 * a + b > s) continue;
        if ((s + 3 * 2 * a + 3 * b - 2 * a - b) % 3 != 0) {
            /* s == 2a+b (mod 3) test below */
        }
        if ((s % 3) != ((2 * a + b) % 3)) continue;
        cnt++;
    }
    return cnt;
}

static uint64_t A268665_count(uint64_t m, uint64_t q, uint64_t p)
{
    uint64_t pr[3], ex[3];
    int np = 0;
    pr[0] = 2; ex[0] = 1; np = 1;
    pr[1] = q; ex[1] = 1; np = 2;
    if (p == q) ex[1] = 2;
    else { pr[2] = p; ex[2] = 1; np = 3; }
    uint64_t divs[32];
    int nd = 1;
    divs[0] = 1;
    for (int i = 0; i < np; i++) {
        int cur = nd;
        uint64_t pw = 1;
        for (uint64_t e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (int j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
        }
    }
    uint64_t total = 0;
    for (int i = 0; i < nd; i++)
        total += A268665_reps(divs[i], m / divs[i]);
    return total;
}

seq_t a(idx_t n)
{
    uint64_t q = A268665_nthprime(n);
    uint64_t target = 4 - (q % 3);
    uint64_t lim = 4 * q * q + 10;
    uint8_t *comp = (uint8_t *)calloc((size_t)lim + 1, 1);
    if (!comp) exit(EXIT_FAILURE);
    uint64_t result = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        if (i % 3 != 1) continue;
        uint64_t m = 2 * q * i;
        if (A268665_count(m, q, i) == target) result++;
    }
    free(comp);
    return (seq_t)result;
}
