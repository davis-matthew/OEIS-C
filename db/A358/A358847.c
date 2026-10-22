/*
 * A358847  a(n) = 1 if A053669(6*n) [the smallest prime not dividing 6*n] is
 *          of the form 6m-1, otherwise a(n) = 0.
 * https://oeis.org/A358847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the smallest prime not dividing 6*n is of the form
 *            6m-1, * otherwise a(n) = 0. Equivalently, find the smallest
 *            prime p ≥ 5 not dividing * n; then a(n) = 1 if p ≡ 5 (mod 6),
 *            else 0
 * Range:     u8 n <= 9979, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Small static list of primes up to 1000 (168 primes). */
static const uint64_t _primes_table[200] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
    73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL,
    127ULL,131ULL,137ULL,139ULL,149ULL,151ULL,157ULL,163ULL,167ULL,
    173ULL,179ULL,181ULL,191ULL,193ULL,197ULL,199ULL,211ULL,223ULL,
    227ULL,229ULL,233ULL,239ULL,241ULL,251ULL,257ULL,263ULL,269ULL,
    271ULL,277ULL,281ULL,283ULL,293ULL,307ULL,311ULL,313ULL,317ULL,
    331ULL,337ULL,347ULL,349ULL,353ULL,359ULL,367ULL,373ULL,379ULL,
    383ULL,389ULL,397ULL,401ULL,409ULL,419ULL,421ULL,431ULL,433ULL,
    439ULL,443ULL,449ULL,457ULL,461ULL,463ULL,467ULL,479ULL,487ULL,
    491ULL,499ULL,503ULL,509ULL,521ULL,523ULL,541ULL,547ULL,557ULL,
    563ULL,569ULL,571ULL,577ULL,587ULL,593ULL,599ULL,601ULL,607ULL,
    613ULL,617ULL,619ULL,631ULL,641ULL,643ULL,647ULL,653ULL,659ULL,
    661ULL,673ULL,677ULL,683ULL,691ULL,701ULL,709ULL,719ULL,727ULL,
    733ULL,739ULL,743ULL,751ULL,757ULL,761ULL,769ULL,773ULL,787ULL,
    797ULL,809ULL,811ULL,821ULL,823ULL,827ULL,829ULL,839ULL,853ULL,
    857ULL,859ULL,863ULL,877ULL,881ULL,883ULL,887ULL,907ULL,911ULL,
    919ULL,929ULL,937ULL,941ULL,947ULL,953ULL,967ULL,971ULL,977ULL,
    983ULL,991ULL,997ULL
};
static const size_t _primes_count = 168;          /* number of primes in the table */

/* Find the first prime ≥5 that does not divide n. */
static uint64_t _smallest_prime_not_dividing(uint64_t n)
{
    /* start at index 2: prime 5 */
    for (size_t i = 2; i < _primes_count; ++i) {
        uint64_t p = _primes_table[i];
        if (n % p != 0) {
            return p;
        }
    }
    /* In practice this point is never reached for n in the intended range.
     * If it occurs we fallback to checking larger primes by trial. */
    uint64_t p = _primes_table[_primes_count - 1] + 2;
    while (true) {
        /* simple primality test */
        bool is_prime = true;
        for (size_t j = 2; j * j <= p; ++j) {
            if (p % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime && n % p != 0) {
            return p;
        }
        p += 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* defined only for n>=1, but guard just in case */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t prime = _smallest_prime_not_dividing(nn);
    /* prime ≡ 5 (mod 6) → 6m-1 */
    return (prime % 6 == 5) ? (seq_t)1 : (seq_t)0;
}
