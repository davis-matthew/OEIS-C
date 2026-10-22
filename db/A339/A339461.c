/*
 * A339461  Number of Fibonacci divisors of n^2 + 1.
 * https://oeis.org/A339461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Fibonacci divisors of n^2 + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this by prime factorising n^2+1, generating all its divisors, and counting those that are Fibonacci numbers. */
/* This method runs in roughly O(τ(n^2+1)) per call, suitable for n up to about 10,000 within 2 seconds. */

#include <stdint.h>
#include <stddef.h>

static uint64_t fibs[100];
static int fibcnt;

/* generate Fibonacci numbers 1,2,3,5,... up to limit */
static void compute_fibs(uint64_t limit)
{
    int i = 0;
    uint64_t a = 1, b = 2;
    while (a <= limit) {
        fibs[i++] = a;
        if (b > limit) break;
        fibs[i++] = b;
        __int128 t = (__int128)a + b;
        a = b;
        b = (uint64_t)t;
    }
    fibcnt = i;
}

/* check whether d is a Fibonacci number (search in fibs array) */
static int is_fib(uint64_t d)
{
    for (int i = 0; i < fibcnt; ++i) {
        if (fibs[i] == d) return 1;
        if (fibs[i] > d) return 0;
    }
    return 0;
}

/* recursive enumeration of divisors */
static void count_divs_rec(const uint64_t *pr, const int *ex, int idx, int total,
                           uint64_t cur, seq_t *cnt, uint64_t limit)
{
    if (idx == total) {
        if (is_fib(cur)) ++(*cnt);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t curp = cur;
    for (int e = 0; e <= ex[idx]; ++e) {
        count_divs_rec(pr, ex, idx + 1, total, curp, cnt, limit);
        curp = curp * p;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute m = n^2 + 1 as uint64_t (fits for n up to 10,000) */
    uint64_t nn = (uint64_t)n;
    uint64_t m = nn * nn + 1;

    /* precompute Fibonacci numbers up to m */
    compute_fibs(m);

    /* factorisation: trial division up to sqrt(m) */
    uint64_t primes[16];
    int exps[16];
    int pfcnt = 0;

    uint64_t temp = m;
    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        primes[pfcnt] = 2;
        exps[pfcnt] = e;
        ++pfcnt;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pfcnt] = p;
            exps[pfcnt] = e;
            ++pfcnt;
        }
    }
    if (temp > 1) {
        primes[pfcnt] = temp;
        exps[pfcnt] = 1;
        ++pfcnt;
    }

    /* enumerate divisors and count Fib ones */
    seq_t cnt = 0;
    count_divs_rec(primes, exps, 0, pfcnt, 1, &cnt, m);

    return cnt;
}
