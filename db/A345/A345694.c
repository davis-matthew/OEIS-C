/*
 * A345694  For 1<=x<=n, 1<=y<=n with gcd(x,y)=1, write 1 = gcd(x,y) = u*x+v*y
 *          with u,v minimal; a(n) = m^2*s, where s is the population variance
 *          of the values of |u| and m is the number of such values.
 * https://oeis.org/A345694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m * Σ|u|² – (Σ|u|)², where * m = number of coprime
 *            ordered pairs (x,y) with 1≤x,y≤n, * and |u| is the minimal
 *            absolute value of the Bézout coefficient * in 1 = u·x + v·y for
 *            gcd(x,y)=1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   mul128_uint64, ui_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary functions -------------------------------------------------------*/
#include <stdint.h>
#include "../utils/mul128_uint64.h"
#include "../utils/ui_gcd.h"

/* modular inverse of a modulo m (gcd(a,m)=1), result in [1,m-1] */
static inline uint64_t modinv(uint64_t a, uint64_t m)
{
    int64_t t = 0, new_t = 1;
    int64_t r = m, new_r = a;
    while (new_r != 0) {
        int64_t q = r / new_r;
        int64_t tmp = t - new_t * q;
        t = new_t;
        new_t = tmp;
        tmp = r - new_r * q;
        r = new_r;
        new_r = tmp;
    }
    if (t < 0)
        t += m;
    return (uint64_t)t;          /* guaranteed to be < m */
}

/* Integer multiplication returning 128‑bit value (if available) */
#ifdef __SIZEOF_INT128__
static inline unsigned __int128 mul128(uint64_t a, uint64_t b)
{
    return (unsigned __int128)a * b;
}
#else

#endif

/* Main function ------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;   /* only pair (1,1) gives |u|=0 */

    uint64_t m    = n;          /* pairs with y = 1 */
    uint64_t sumAbs = 0;
    uint64_t sumSq  = 0;

    for (uint64_t y = 2; y <= n; ++y) {
        /* residues r from 1 to min(y-1,n) are the only ones that can appear */
        uint64_t maxr = (y - 1 < n) ? y - 1 : n;
        for (uint64_t r = 1; r <= maxr; ++r) {
            if (ui_gcd(r, y) != 1)
                continue;

            /* minimal absolute Bézout coefficient for residue r */
            uint64_t inv = modinv(r, y);          /* 1 <= inv < y */
            uint64_t min_u = (inv <= y - inv) ? inv : y - inv;

            /* number of x with this residue in [1,n] */
            uint64_t count = (n - r) / y + 1;

            sumAbs += min_u * count;
            sumSq  += min_u * min_u * count;
            m      += count;
        }
    }

    /* a(n) = m * sumSq – sumAbs² */
#ifdef __SIZEOF_INT128__
    unsigned __int128 prod   = mul128(m, sumSq);
    unsigned __int128 sqAbs  = mul128(sumAbs, sumAbs);
    unsigned __int128 res128 = prod - sqAbs;
    return (seq_t)res128;
#else
    /* 64‑bit build: intermediate fits into 128‑bit via long double cast */
    unsigned long long prod = m * sumSq;
    unsigned long long sqAbs = sumAbs * sumAbs;
    return (seq_t)(prod - sqAbs);
#endif
}
