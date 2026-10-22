/*
 * A328615  Number of digits larger than 1 in primorial base expansion of n.
 * https://oeis.org/A328615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of digits larger than 1 in the primorial base
 *            expansion of n
 * Range:     u8 n <= 9381, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t _primes[200] = {
   2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,41ULL,43ULL,47ULL,
   53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL,
   127ULL,131ULL,137ULL,139ULL,149ULL,151ULL,157ULL,163ULL,167ULL,173ULL,179ULL,181ULL,191ULL,
   193ULL,197ULL,199ULL,211ULL,223ULL,227ULL,229ULL,233ULL,239ULL,241ULL,251ULL,257ULL,263ULL,
   269ULL,271ULL,277ULL,281ULL,283ULL,293ULL,307ULL,311ULL,313ULL,317ULL,331ULL,337ULL,347ULL,
   349ULL,353ULL,359ULL,367ULL,373ULL,379ULL,383ULL,389ULL,397ULL,401ULL,409ULL,419ULL,421ULL,
   431ULL,433ULL,439ULL,443ULL,449ULL,457ULL,461ULL,463ULL,467ULL,479ULL,487ULL,491ULL,499ULL,
   503ULL,509ULL,521ULL,523ULL,541ULL,547ULL,557ULL,563ULL,569ULL,571ULL,577ULL,587ULL,593ULL,
   599ULL,601ULL,607ULL,613ULL,617ULL,619ULL,631ULL,641ULL,643ULL,647ULL,653ULL,659ULL,661ULL,
   673ULL,677ULL,683ULL,691ULL,701ULL,709ULL,719ULL,727ULL,733ULL,739ULL,743ULL,751ULL,757ULL,
   761ULL,769ULL,773ULL,787ULL,797ULL,809ULL,811ULL,821ULL,823ULL,827ULL,829ULL,839ULL,853ULL,
   857ULL,859ULL,863ULL,877ULL,881ULL,883ULL,887ULL,907ULL,911ULL,919ULL,929ULL,937ULL,941ULL,
   947ULL,953ULL,967ULL,971ULL,977ULL,983ULL,991ULL,997ULL
};

seq_t a(idx_t n)
{
    /* Build primorials <= n */
    idx_t primorials[200];
    idx_t prim_cnt = 1;          /* primorials[0] = 1 always */
    primorials[0] = (idx_t)1;

    idx_t primes_cnt = sizeof(_primes) / sizeof(_primes[0]);

    for (idx_t i = 0; i < primes_cnt && prim_cnt < 200; ++i) {
        /* Avoid overflow: if current * prime > n, stop */
        if (primorials[prim_cnt-1] > n / _primes[i])
            break;
        primorials[prim_cnt++] = primorials[prim_cnt-1] * _primes[i];
    }

    idx_t remaining = n;
    idx_t count = 0;

    /* Greedy conversion to primorial base */
    for (idx_t j = prim_cnt; j > 0; --j) {
        idx_t p = primorials[j-1];
        idx_t d = remaining / p;
        if (d > 1)
            ++count;
        remaining -= d * p;
    }

    return (seq_t)count;
}
