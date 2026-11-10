/*
 * A389105  a(n) is the smallest value of k such that gcd(A381466(k-1), k) =
 *          n.
 * https://oeis.org/A389105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. Simulate A381466 (a(0)=4; a(k)=a(k-1)+k if
 *            gcd(a(k-1),k)=1, else a(k)=k/gcd(a(k-1),k)) and return the first
 *            k with gcd(A381466(k-1),k)=n. Speed-up: after a division at step
 *            m by g (value m/g), every common odd prime p of k=m+j and
 *            A381466(k-1) divides j*(g*(j-1)+2), and j=1 never divides
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A389105_LIM 65536

typedef struct {
    uint64_t k;   /* last processed step */
    uint64_t v;   /* A381466(k) */
    uint64_t m;   /* step of the last division (0 = none yet) */
    uint64_t g;   /* gcd used at that division */
} A389105_state;

static uint64_t A389105_first[A389105_LIM];
static A389105_state A389105_S = {0, 4, 0, 0};

static uint64_t A389105_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static unsigned A389105_ctz(uint64_t x)
{
    unsigned c = 0;
    if (x == 0) return 64;
#if defined(__GNUC__)
    c = (unsigned)__builtin_ctzll(x);
#else
    while (!(x & 1)) { x >>= 1; c++; }
#endif
    return c;
}

/* part of *rem built from primes of r; removes it from *rem */
static uint64_t A389105_take(uint64_t *rem, uint64_t r)
{
    uint64_t kp = 1, x = A389105_gcd(*rem, r);
    while (x > 1) {
        kp *= x;
        *rem /= x;
        x = A389105_gcd(*rem, x);
    }
    return kp;
}

/* advance one step; returns the gcd used at that step */
static uint64_t A389105_step(A389105_state *s)
{
    uint64_t k = ++s->k, v = s->v, g;
    if (s->m == 0) {
        g = A389105_gcd(v, k);
    } else {
        uint64_t j = k - s->m;
        if (j == 1) {
            g = 1;
        } else if (s->g > (UINT64_MAX - 2) / (j - 1)) {
            g = A389105_gcd(v, k);
        } else {
            unsigned tk = A389105_ctz(k), tv = A389105_ctz(v);
            unsigned tz = tk < tv ? tk : tv;
            uint64_t kodd = k >> tk;
            uint64_t jo = j >> A389105_ctz(j);
            uint64_t x = s->g * (j - 1) + 2;
            uint64_t xo = x >> A389105_ctz(x);
            uint64_t r1 = 1, r2 = 1, odd = 1;
            if (jo > 1) r1 = A389105_gcd(jo, kodd % jo);
            if (xo > 1) r2 = A389105_gcd(xo, kodd % xo);
            if (r1 > 1 || r2 > 1) {
                uint64_t rem = kodd, kp = 1;
                if (r1 > 1) kp *= A389105_take(&rem, r1);
                if (r2 > 1) kp *= A389105_take(&rem, r2);
                odd = A389105_gcd(kp, v % kp);
            }
            g = odd << tz;
        }
    }
    if (g == 1) {
        s->v = v + k;
    } else {
        s->v = k / g;
        s->m = k;
        s->g = g;
    }
    return g;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    if (target == 0) return (seq_t)0;

    if (target < A389105_LIM) {
        if (A389105_first[target])
            return (seq_t)A389105_first[target];
        for (;;) {
            uint64_t g = A389105_step(&A389105_S);
            if (g < A389105_LIM && !A389105_first[g])
                A389105_first[g] = A389105_S.k;
            if (g == target)
                return (seq_t)A389105_S.k;
        }
    }

    /* large target: fresh independent search */
    {
        A389105_state s;
        s.k = 0; s.v = 4; s.m = 0; s.g = 0;
        for (;;) {
            uint64_t g = A389105_step(&s);
            if (g == target)
                return (seq_t)s.k;
        }
    }
}
