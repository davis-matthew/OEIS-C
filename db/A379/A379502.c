/*
 * A379502  Characteristic function of almost Zumkeller numbers: a(n) = 1 if
 *          Zumkeller-deficiency of n (A103977) is 1, otherwise 0.
 * https://oeis.org/A379502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the Zumkeller‑deficiency of n is 1, * 0 otherwise.
 *            (Definition from A103977)
 * Range:     u8 n <= 9544, u16 n <= 9544, u32 n <= 9544, u64 n <= 9544,
 *            u128 n <= 9544, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------
 *  Generate all positive divisors of n from its factorisation.
 *-------------------------------------------------------------*/
static void gen_divs_rec(uint64_t cur, size_t i,
                         uint64_t primes[], uint64_t exps[], size_t cnt,
                         uint64_t *divs, size_t *divc)
{
    if (i == cnt) {
        divs[(*divc)++] = cur;
        return;
    }
    uint64_t p = primes[i];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exps[i]; ++e) {
        gen_divs_rec(cur * mult, i + 1, primes, exps, cnt, divs, divc);
        mult *= p;
    }
}

/*-------------------------------------------------------------
 *  Main sequence function
 *-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /*----------------------------------------------------------
     * 1. Prime factorisation of n (simple trial division)
     *----------------------------------------------------------*/
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;

    uint64_t tmp = N;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes[pf_cnt]   = p;
            exps[pf_cnt]     = cnt;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {                      /* remaining prime factor */
        primes[pf_cnt]   = tmp;
        exps[pf_cnt]     = 1;
        ++pf_cnt;
    }

    /*----------------------------------------------------------
     * 2. Compute σ(n): sum of all divisors
     *----------------------------------------------------------*/
    uint64_t sigma = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t term = 1;   /* (p^(e+1)-1)/(p-1) */
        uint64_t mult = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) {
            mult *= primes[i];
            term += mult;
        }
        sigma *= term;
    }

    /* sigma is now a 64‑bit integer (fits for n ≤ 10 000). */
    /*----------------------------------------------------------
     * 3. Generate all divisors
     *----------------------------------------------------------*/
    uint64_t divs[512];
    size_t divc = 0;
    gen_divs_rec(1, 0, primes, exps, pf_cnt, divs, &divc);

    /*----------------------------------------------------------
     * 4. Subset‑sum DP using a bitset (one bit per achievable sum)
     *----------------------------------------------------------*/
    size_t bits_len = sigma + 1;             /* we need bits up to sigma (inclusive) */
    size_t words    = (bits_len >> 6) + 1;   /* each word holds 64 bits */

    uint64_t *bits   = (uint64_t*)calloc(words, sizeof(uint64_t));
    if (!bits) return (seq_t)0;             /* memory failure → safe result */
    uint64_t *tmpbits = (uint64_t*)malloc(words * sizeof(uint64_t));
    if (!tmpbits) {
        free(bits);
        return (seq_t)0;
    }

    bits[0] = 1ULL;  /* sum 0 is always achievable */

    for (size_t idx = 0; idx < divc; ++idx) {
        uint64_t d = divs[idx];
        memcpy(tmpbits, bits, words * sizeof(uint64_t));

        size_t wshift = d >> 6;
        unsigned bshift = d & 63;

        if (bshift == 0) {
            for (size_t j = words; j-- > wshift; ) {
                bits[j] |= tmpbits[j - wshift];
            }
        } else {
            for (size_t j = words; j-- > wshift + 1; ) {
                bits[j] |= (tmpbits[j - wshift] << bshift)
                         | (tmpbits[j - wshift - 1] >> (64 - bshift));
            }
            if (wshift < words) {
                bits[wshift] |= (tmpbits[0] << bshift);
            }
        }
    }

    /*----------------------------------------------------------
     * 5. Find largest achievable sum ≤ σ(n)/2
     *----------------------------------------------------------*/
    size_t half = sigma >> 1;          /* floor(σ/2) */
    size_t wordIdx = half >> 6;
    unsigned bitIdx = half & 63;

    size_t best = 0;
    bool found = false;

    if (wordIdx < words) {
        uint64_t word = bits[wordIdx];
        uint64_t mask = (bitIdx == 63) ? ~0ULL : ((1ULL << (bitIdx + 1)) - 1);
        word &= mask;
        if (word) {
            int pos = 63 - __builtin_clzll(word);
            best = (wordIdx << 6) + pos;
            found = true;
        }
    }
    if (!found) {
        while (wordIdx--) {
            uint64_t word = bits[wordIdx];
            if (word) {
                int pos = 63 - __builtin_clzll(word);
                best = (wordIdx << 6) + pos;
                found = true;
                break;
            }
        }
    }

    free(tmpbits);
    free(bits);

    /*----------------------------------------------------------
     * 6. Compute deficiency and return 1 iff it equals 1
     *----------------------------------------------------------*/
    uint64_t deficiency = sigma - (2u * best);
    return (seq_t)(deficiency == 1 ? 1 : 0);
}
