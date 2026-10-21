/*
 * A296929  Rational primes that decompose in the field Q(sqrt(-17)).
 * https://oeis.org/A296929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th rational prime that splits in Q(√‑17), i.e. the
 *            n‑th prime p (> 2, p≠17) with (-17|p)=1, equivalently p ≡
 *            1,3,7,9,11,13,21,23,25,27,31,33,39,49,53,63 (mod 68)
 * Range:     u8 n <= 23, u16 n <= 3264, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static const unsigned char split_ok[68] = {
    0, /* 0 */
    1, /* 1 */
    0, /* 2 */
    1, /* 3 */
    0, /* 4 */
    0, /* 5 */
    0, /* 6 */
    1, /* 7 */
    0, /* 8 */
    1, /* 9 */
    0, /*10*/
    1, /*11*/
    0, /*12*/
    1, /*13*/
    0, /*14*/
    0, /*15*/
    0, /*16*/
    0, /*17*/
    0, /*18*/
    0, /*19*/
    0, /*20*/
    1, /*21*/
    0, /*22*/
    1, /*23*/
    0, /*24*/
    1, /*25*/
    0, /*26*/
    1, /*27*/
    0, /*28*/
    0, /*29*/
    0, /*30*/
    1, /*31*/
    0, /*32*/
    1, /*33*/
    0, /*34*/
    0, /*35*/
    0, /*36*/
    0, /*37*/
    0, /*38*/
    1, /*39*/
    0, /*40*/
    0, /*41*/
    0, /*42*/
    0, /*43*/
    0, /*44*/
    0, /*45*/
    0, /*46*/
    0, /*47*/
    0, /*48*/
    1, /*49*/
    0, /*50*/
    0, /*51*/
    0, /*52*/
    1, /*53*/
    0, /*54*/
    0, /*55*/
    0, /*56*/
    0, /*57*/
    0, /*58*/
    0, /*59*/
    0, /*60*/
    0, /*61*/
    0, /*62*/
    1, /*63*/
    0, /*64*/
    0, /*65*/
    0, /*66*/
    0  /*67*/
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* behaviour below offset is irrelevant */

    idx_t count     = 0;
    idx_t candidate = 3;   /* start with the first odd prime */

    for (;;)
    {
        if (is_prime(candidate))
        {
            if (candidate != 17 && split_ok[(size_t)(candidate % 68)])
            {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate += 2;    /* stay odd */
    }
}
