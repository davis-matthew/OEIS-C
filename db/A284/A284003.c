/*
 * A284003  a(n) = A007913(A283477(n)) = A019565(A006068(n)).
 * https://oeis.org/A284003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of primes at positions set in the binary
 *            representation of m(n), where m(0) = 0 and for n > 0 m(n) =
 *            (2^{k+1} − 1) − m(n – 2^k), with k = ⌊log₂ n⌋
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   hp_pow2, m_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/hp_pow2.h"
#include "../utils/m_value.h"

/* Prime numbers: the first many primes are listed explicitly to
   enable multiplication by any required bit position. */
static const uint64_t PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL,
    109ULL, 113ULL, 127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL,
    157ULL, 163ULL, 167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL,
    197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL,
    241ULL, 251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
    283ULL, 293ULL, 307ULL, 311ULL, 313ULL, 317ULL, 331ULL, 337ULL,
    347ULL, 349ULL, 353ULL, 359ULL, 367ULL, 373ULL, 379ULL, 383ULL,
    389ULL, 397ULL, 401ULL, 409ULL, 419ULL, 421ULL, 431ULL, 433ULL,
    439ULL, 443ULL, 449ULL, 457ULL, 461ULL, 463ULL, 467ULL, 479ULL,
    487ULL, 491ULL, 499ULL, 503ULL, 509ULL, 521ULL, 523ULL, 541ULL,
    547ULL, 557ULL, 563ULL, 569ULL, 571ULL, 577ULL, 587ULL, 593ULL,
    599ULL, 601ULL, 607ULL, 613ULL, 617ULL, 619ULL, 631ULL, 641ULL,
    643ULL, 647ULL, 653ULL, 659ULL, 661ULL, 673ULL, 677ULL, 683ULL,
    691ULL, 701ULL, 709ULL, 719ULL, 727ULL, 733ULL, 739ULL, 743ULL,
    751ULL, 757ULL, 761ULL, 769ULL, 773ULL, 787ULL, 797ULL, 809ULL,
    811ULL, 821ULL, 823ULL, 827ULL, 829ULL, 839ULL, 853ULL, 857ULL,
    859ULL, 863ULL, 877ULL, 881ULL, 883ULL, 887ULL, 907ULL, 911ULL,
    919ULL, 929ULL, 937ULL, 941ULL, 947ULL, 953ULL, 967ULL, 971ULL,
    977ULL, 983ULL, 991ULL, 997ULL
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t m = m_value(n);
    seq_t prod = 1;
    idx_t bit = 0;
    while (m) {
        if (m & 1ULL) prod *= PRIMES[bit];
        m >>= 1;
        ++bit;
    }
    return prod;
}
