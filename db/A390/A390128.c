/*
 * A390128  Biquadratefree numbers that are neither cubefree nor squareful.
 * https://oeis.org/A390128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc, better_than_seq_t, freep
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A390128:
   Biquadratefree numbers that are neither cubefree nor squareful.
   A(n) is the nth smallest integer whose prime factor exponents
   lie in {1,2,3}, with at least one exponent equal to 3 and
   at least one exponent equal to 1. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc.h"
#include "../utils/better_than_seq_t.h"
#include "../utils/freep.h"

static struct {
    seq_t *vals;   /* cached values, 1‑based indexing in cache[n-1] */
    size_t size;
    size_t capacity;
    uint32_t max_tested;   /* maximum integer for which we have checked */
} cache = {NULL,0,0,0};

/* Ensure that cache contains at least n entries */
static void ensure_cache(idx_t n){
    if(cache.size >= (size_t)n) return;

    /* Estimate a bound for the sieve; start from current limit or increase */
    uint32_t limit = cache.max_tested ? cache.max_tested : 1000;
    if(limit < 10) limit = 10;
    /* Rough upper bound: at most n*200 integers are needed, double if necessary */
    if((uint64_t)limit < (uint64_t)n * 200) limit = (uint32_t)(n * 200);
    if(limit < 1000) limit = 1000;

    for(;;){
        /* If we have already computed up to 'limit', skip recomputation */
        if(limit <= cache.max_tested){
            if(cache.size >= (size_t)n) return;
        }
        /* Resize spool for new limit if needed */
        uint32_t *spf = (uint32_t*)alloc((limit + 1) * sizeof(uint32_t));
        for(uint32_t i=0;i<=limit;i++) spf[i]=0;

        for(uint32_t i=2;i<=limit;i++){
            if(spf[i]==0){
                spf[i]=i;
                if((uint64_t)i * i <= limit){
                    for(uint64_t j=(uint64_t)i*i;j<=limit;j+=i){
                        if(spf[j]==0) spf[j]=i;
                    }
                }
            }
        }

        /* Compute new values starting just after current max_tested */
        uint32_t start = cache.max_tested + 1;
        for(uint32_t val = start; val <= limit; ++val){
            uint32_t tmp = val;
            uint8_t min_exp = 4, max_exp = 0;
            int valid = 1;
            while(tmp > 1 && valid){
                uint32_t p = spf[tmp];
                uint8_t cnt = 0;
                while(tmp % p == 0){
                    tmp /= p;
                    ++cnt;
                    if(cnt > 3){ valid = 0; break; }
                }
                if(!valid) break;
                if(cnt < min_exp) min_exp = cnt;
                if(cnt > max_exp) max_exp = cnt;
            }
            if(valid && max_exp == 3 && min_exp == 1){
                if(cache.size == cache.capacity){
                    size_t new_cap = (cache.capacity==0)?64:cache.capacity*2;
                    seq_t *new_vals = (seq_t*)alloc(new_cap * sizeof(seq_t));
                    if(cache.vals) {for(size_t i=0;i<cache.size;i++) new_vals[i]=cache.vals[i]; freep(cache.vals);}
                    cache.vals = new_vals;
                    cache.capacity = new_cap;
                }
                cache.vals[cache.size++] = (seq_t)val;
            }
        }
        freep(spf);
        cache.max_tested = limit;

        if(cache.size >= (size_t)n) return;
        /* Not enough values yet: increase limit */
        limit *= 2;
    }
}

seq_t a(idx_t n){
    if(n==0) return 0; /* undefined for n=0 but guard */
    ensure_cache(n);
    return cache.vals[n-1];
}
