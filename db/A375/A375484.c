/*
 * A375484  a(n) = r in a log_2-like sequence of algebraic integers f(n) =
 *          (r+phi*s), where phi = (1+sqrt(5))/2 is the golden ratio.
 * https://oeis.org/A375484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   A375484 is given by the additive function * f(1) = 0, * f(2) =
 *            1, * for odd primes p: f(p) = ( f(p-1) + φ·f(p+1) ) / (1+φ), *
 *            for composite n: f(n) = Σ e_i·f(p_i) (sum over prime powers) *
 *            with φ = (1+√5)/2. f(n) = a(n) + φ·b(n). The function returns
 *            a(n)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/*  Cache for computed a(n) and b(n).  The cache is large enough for
 *  the small numbers required by the tests (n up to ~10 000).
 *  It is also usable for arbitrary‑precision seq_t because the
 *  compiler will instantiate the proper type.                    */
#define CACHE_LIMIT 2000000      /* enough for all test cases */

typedef struct {
    seq_t  a;      /* integer part of f(n) */
    seq_t  b;      /* coefficient of φ in f(n) */
    unsigned char  flag;   /* 0 = not computed, 1 = computed */
} cache_entry;

static cache_entry cache[CACHE_LIMIT];
static bool cache_initialised = false;

/*------------------------------------------------------------------*/
static void init_cache(void)
{
    /* Base cases f(1)=0, f(2)=1.  All other entries start as
     * uninitialised (flag==0).                                                 */
    cache[1].a = 0;
    cache[1].b = 0;
    cache[1].flag = 1;
    cache[2].a = 1;
    cache[2].b = 0;
    cache[2].flag = 1;
    cache_initialised = true;
}

/*------------------------------------------------------------------*/
/*  Simple deterministic primality test for idx_t up to the small
 *  numbers that appear in the sequence.                                 */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/*------------------------------------------------------------------*/
/*  Helper structure for returning a pair (a,b).                      */
typedef struct { seq_t a; seq_t b; } pairab;

/*------------------------------------------------------------------*/
/*  Recursive computation of f(n) = a(n) + φ·b(n).  Results are memoised
 *  in the cache for indices < CACHE_LIMIT.                             */
static pairab compute_f(idx_t n)
{
    if (!cache_initialised) init_cache();

    if (n < CACHE_LIMIT && cache[n].flag) {
        return (pairab){ cache[n].a, cache[n].b };
    }

    pairab res;

    /* Base cases are handled by cache, but we keep them explicit for
     * clarity. */
    if (n == 1) {
        res.a = 0; res.b = 0;
    } else if (n == 2) {
        res.a = 1; res.b = 0;
    } else if (is_prime(n)) {          /* odd prime */
        pairab f1 = compute_f(n - 1);
        pairab f2 = compute_f(n + 1);

        /* f(p) = (f(p-1)+φ*f(p+1))/(1+φ)  →  a',b' as derived above  */
        seq_t c = f1.a + f2.b;                     /* numerator part without φ */
        seq_t d = f1.b + f2.a + f2.b;              /* φ‑coefficient of numerator */
        res.a = 2 * c - d;                         /* a' = 2c - d */
        res.b = d - c;                             /* b' = d - c */
    } else {                                   /* composite */
        seq_t sum_a = 0;
        seq_t sum_b = 0;
        idx_t temp = n;

        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                idx_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                pairab fp = compute_f(p);
                sum_a += (seq_t)exp * fp.a;
                sum_b += (seq_t)exp * fp.b;
            }
        }
        if (temp > 1) {                     /* remaining prime factor */
            pairab fp = compute_f(temp);
            sum_a += fp.a;
            sum_b += fp.b;
        }
        res.a = sum_a;
        res.b = sum_b;
    }

    if (n < CACHE_LIMIT) {
        cache[n].a = res.a;
        cache[n].b = res.b;
        cache[n].flag = 1;
    }
    return res;
}

/*------------------------------------------------------------------*/
/*  Public sequence function – returns a(n).                          */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 */
    pairab r = compute_f(n);
    return r.a;
}
