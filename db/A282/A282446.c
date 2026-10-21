/*
 * A282446  Call d a recursive divisor of n iff the p-adic valuation of d is a
 *          recursive divisor of the p-adic valuation of n for any prime p
 *          dividing d; a(n) gives the number of recursive divisors of n.
 * https://oeis.org/A282446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative; for prime power a(p^k)=1+a(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t calc_a_recursive(uint64_t m){
    if(m==1) return (seq_t)1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for(size_t i=0;i<cnt;i++){
        seq_t term = (seq_t)1 + calc_a_recursive(exps[i]);
        result *= term;
    }
    return result;
}

seq_t a(idx_t n){
    return calc_a_recursive((uint64_t)n);
}
