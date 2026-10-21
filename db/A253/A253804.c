/*
 * A253804  a(n) gives the odd leg of the second of the two Pythagorean
 *          triangles with hypotenuse A080109(n) = A002144(n)^2. This is the
 *          larger of the two possible odd legs.
 * https://oeis.org/A253804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let p be the n-th prime ≡ 1 (mod 4) and p = x² + y² (x > y > 0).
 *            Set A = x² – y² and B = 2xy. The two odd legs of the two
 *            Pythagorean triangles with hypotenuse p² are p·A and |B² – A²|.
 *            a(n) is the larger of these two odd legs
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

/* cache of primes ≡ 1 (mod 4) */
static uint64_t *primes1 = NULL;
static size_t    primes1_cnt = 0;   /* number of cached 1 (mod 4) primes */

/* ensure that at least n such primes are cached */
static void ensure_primes1(idx_t n)
{
    if (primes1_cnt >= (size_t)n)
        return;

    /* choose a sieve limit that is guaranteed to contain at least n primes
       ≡ 1 (mod 4).  The bound n*20 is safe for n up to several thousand. */
    size_t limit = (size_t)n * 20;
    if (limit < 2000)
        limit = 2000;
    /* the sieve may be heavy, so rebuild from scratch each time n increases */
    if (primes1) {
        free(primes1);
        primes1 = NULL;
        primes1_cnt = 0;
    }

    size_t cnt;
    uint64_t *all = NULL;
    build_primes_upto(limit, &all, &cnt);      /* all primes ≤ limit */

    primes1 = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes1) abort();                     /* allocation failed */

    for (size_t i = 0; i < cnt; ++i) {
        if ((all[i] & 3ULL) == 1) {            /* prime ≡ 1 mod 4 */
            primes1[primes1_cnt++] = all[i];
        }
    }
    free(all);
}

/* compute r such that r² ≡ –1 (mod p) for a prime p ≡ 1 (mod 4) */
static seq_t sqrt_minus_one_mod(uint64_t p)
{
    /* find a quadratic non‑residue a modulo p */
    for (uint64_t a = 2; ; ++a) {
        seq_t leg = mod_pow((seq_t)a, (p - 1) / 2, (seq_t)p);
        if (leg == (seq_t)(p - 1)) {          /* Legendre symbol = –1 */
            /* then a^((p–1)/4) is a square root of –1 */
            return mod_pow((seq_t)a, (p - 1) / 4, (seq_t)p);
        }
    }
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* obtain the n‑th prime ≡ 1 (mod 4) */
    ensure_primes1(n);
    uint64_t p = primes1[(size_t)n - 1];
    seq_t sp = (seq_t)p;

    /* find representation p = x² + y² using Cornacchia’s algorithm */
    seq_t t = sqrt_minus_one_mod(p);          /* sqrt(–1) mod p   */
    seq_t r0 = t, r1 = sp;
    while (r1 * r1 > sp) {
        seq_t r2 = r0 % r1;
        r0 = r1;
        r1 = r2;
    }

    seq_t x = r1;                            /* x ≤ sqrt(p)      */
    seq_t y2 = sp - x * x;
    seq_t y = floor_sqrt(y2);                /* floor(sqrt(y2))   */
    /* y should be an integer: y*y == y2  (always holds for prime p≡1mod4) */

    /* first triangle parameters */
    seq_t A = x * x - y * y;                 /* x² – y²          */
    seq_t B = 2 * x * y;                     /* 2xy              */

    /* two odd legs of the two triangles */
    seq_t leg1 = sp * A;                     /* p·(x² – y²)      */
    seq_t B2 = B * B;
    seq_t A2 = A * A;
    seq_t leg2 = (B2 > A2) ? (B2 - A2) : (A2 - B2);  /* |B² – A²|  */

    return (leg1 > leg2) ? leg1 : leg2;     /* larger odd leg   */
}
