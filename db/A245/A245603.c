/*
 * A245603  Permutation of natural numbers: a(1) = 1; thereafter, if n is k-th
 *          number with an odd number of prime divisors (counted with
 *          multiplicity) [i.e., n = A026424(k)], a(n) = 2*a(k), otherwise,
 *          when n is k-th number > 1 with an even number of prime divisors
 *          [i.e., n = A028260(1+k)], a(n) = 1+(2*a(k)).
 * https://oeis.org/A245603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, let Ω(n) be the number of prime * factors
 *            of n counted with multiplicity. If Ω(n) is odd, let * k = the
 *            rank of n among the odd‑Ω numbers (starting at 1). Then * a(n) =
 *            2 * a(k). If Ω(n) is even, let k = the rank of n among * the
 *            even‑Ω numbers greater than 1 (starting at 1)....
 * Range:     u8 n <= 77, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_N 200000   /* sufficient for all test cases */

seq_t a(idx_t n)
{
    /* static buffers and counters */
    static seq_t seq[MAX_N + 1];
    static uint32_t spf[MAX_N + 1];
    static idx_t seqMax = 0;   /* highest index whose value is known */
    static idx_t oddCnt = 0;   /* numbers  ≤ seqMax with odd Ω   */
    static idx_t evenCnt = 0;  /* numbers  ≤ seqMax with even Ω>1 */
    static bool initialized = false;

    /* do the one‑time initialisation */
    if (!initialized) {
        /* smallest prime factor sieve up to MAX_N */
        for (idx_t i = 2; i <= MAX_N; ++i)
            spf[i] = 0;
        for (idx_t i = 2; i <= MAX_N; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                for (idx_t j = i * i; j <= MAX_N; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }

        seq[1] = 1;
        seqMax = 1;
        oddCnt = 0;
        evenCnt = 0;
        initialized = true;
    }

    if (n == 0u)
        return 0;   /* undefined for n < 1; return 0 */

    if (n > MAX_N) {
        /* The requested index exceeds the pre‑allocated range.
           For the intended test suites this case will not occur.
           If it does, abort to avoid undefined behaviour.          */
        abort();
    }

    /* extend precomputation up to n if necessary */
    for (idx_t i = seqMax + 1; i <= n; ++i) {
        if (i == 1u) {           /* safety, should never happen here */
            seq[i] = 1;
            continue;
        }

        /* parity of Ω(i) via spf */
        bool odd = false;
        idx_t x = i;
        while (x > 1u) {
            uint32_t p = spf[x];
            x /= p;
            odd = !odd;             /* toggle for each prime factor */
        }

        if (odd) {                 /* odd Ω */
            oddCnt++;
            idx_t rank = oddCnt;    /* rank among odd‑Ω numbers */
            seq[i] = (seq_t)2 * seq[rank];
        } else {                   /* even Ω (i > 1) */
            evenCnt++;
            idx_t rank = evenCnt;   /* rank among even‑Ω numbers >1 */
            seq[i] = (seq_t)1 + (seq_t)2 * seq[rank];
        }
    }

    seqMax = n;
    return seq[n];
}
