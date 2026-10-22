/*
 * A365102  Number of distinct residues of x^n (mod n^5), x=0..n^5-1.
 * https://oeis.org/A365102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of x^n (mod n^5), x=0..n^5-1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation factors n, then for each prime power p^a dividing n
    computes the number of residues modulo p^{5a} using Chinese
    remainder theorem.  For each prime power the number of
    distinct residues equals
          1 + Σ_{v:0 ≤ v < 5a/n} image_size(p, 5a - n*v, n)
    where image_size(p,b,e) is the size of the image of the map
    u → u^e on the unit group modulo p^b.  For odd p the unit group is
    cyclic, for 2^b (b≥3) it is C2 × C_{2^{b-2}}.  The formulae are
    applied accordingly.  All arithmetic uses seq_t, which may be
    any unsigned integer type (including arbitrary precision). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

static seq_t units_image_size(seq_t p, idx_t b, idx_t e)
{
    /*  p is odd or 2,  b ≥ 1,  e ≥ 1  */
    if (p > (seq_t)2) {
        seq_t phi = power_seq(p, b - 1);
        phi *= (p - (seq_t)1);
        seq_t g = gcd(phi, (seq_t)e);
        return phi / g;
    }

    /* p == 2 */
    if (b == (idx_t)1)         return (seq_t)1;      /* φ(2)=1  */
    if (b == (idx_t)2) {
        seq_t phi = (seq_t)2;          /* φ(4)=2  */
        seq_t g = gcd(phi, (seq_t)e);
        return phi / g;
    }

    /* b ≥ 3: unit group ≅ C2 × C_{2^{b-2}} */
    seq_t M = power_seq((seq_t)2, b - (idx_t)2);     /* 2^{b-2}   */
    seq_t gM = gcd(M, (seq_t)e);
    seq_t comp = M / gM;                             /* component size */

    if ((e & 1) == 0)    /* e even → C2 component collapses to 1   */
        return comp;
    else                 /* e odd  → C2 component keeps size 2     */
        return comp * (seq_t)2;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but harmless */
    idx_t orig_n = n;

    /* Small prime table up to √10000 = 100 */
    static const int small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97
    };
    const size_t SMALL_CNT = sizeof(small_primes)/sizeof(small_primes[0]);

    /* Factorization */
    int ptab[25];
    int exptab[25];
    size_t pcount = 0;

    for (size_t i = 0; i < SMALL_CNT; ++i) {
        int p = small_primes[i];
        if ( (idx_t)p * (idx_t)p > n) break;
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            ptab[pcount] = p;
            exptab[pcount] = e;
            ++pcount;
        }
    }
    if (n > 1) {   /* remaining prime factor > 100 */
        ptab[pcount] = (int)n;
        exptab[pcount] = 1;
        ++pcount;
    }

    /* Compute product over prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcount; ++i) {
        idx_t p = (idx_t)ptab[i];
        idx_t a = (idx_t)exptab[i];
        idx_t k = (idx_t)(5 * a);              /* exponent of modulus p^k = p^{5a} */

        idx_t max_v = (k - (idx_t)1) / orig_n;  /* v such that e*v < k */
        seq_t sum_units = (seq_t)0;

        for (idx_t v = 0; v <= max_v; ++v) {
            idx_t b = k - orig_n * v;          /* exponent of current modulus p^b */
            seq_t img = units_image_size((seq_t)p, b, orig_n);
            sum_units += img;
        }

        seq_t count = sum_units + (seq_t)1;   /* add zero residue */
        result *= count;
    }

    return result;
}
