/*
 * A373411  Sum of the n-th maximal antirun of squarefree numbers differing by
 *          more than one.
 * https://oeis.org/A373411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n‑th maximal antirun * of squarefree numbers
 *            differing by >1
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    /* Initial estimate for an upper bound on the needed numbers */
    size_t bound = n * 12 + 100;
    if (bound < 200) bound = 200;

    while (1) {
        /* Allocate array flagging squarefree numbers up to bound */
        unsigned char *sqf = (unsigned char *)malloc(bound + 1);
        if (!sqf) return 0; /* allocation failure, unlikely */
        for (size_t i = 1; i <= bound; ++i) sqf[i] = 1;
        sqf[0] = 0; /* 0 is not squarefree */

        /* Sieve to cross out multiples of prime squares */
        size_t sqrtb = (size_t)(sqrt((double)bound) + 1.0);
        unsigned char *isPrime = (unsigned char *)malloc(sqrtb + 1);
        if (!isPrime) { free(sqf); return 0; }
        for (size_t i = 0; i <= sqrtb; ++i) isPrime[i] = 1;
        isPrime[0] = isPrime[1] = 0;
        for (size_t i = 2; i * i <= sqrtb; ++i) {
            if (isPrime[i]) {
                for (size_t j = i * i; j <= sqrtb; j += i)
                    isPrime[j] = 0;
            }
        }
        for (size_t i = 2; i <= sqrtb; ++i) {
            if (isPrime[i]) {
                size_t p2 = i * i;
                for (size_t j = p2; j <= bound; j += p2)
                    sqf[j] = 0;
            }
        }
        free(isPrime);

        /* Scan squarefree numbers, building antiruns */
        seq_t runSum = 0;
        seq_t prev = 0;
        bool havePrev = false;
        idx_t runCnt = 0;

        for (size_t num = 1; num <= bound; ++num) {
            if (!sqf[num]) continue;
            seq_t cur = (seq_t)num;
            if (!havePrev) {
                runSum = cur;
                prev = cur;
                havePrev = true;
            } else {
                seq_t diff = cur - prev;
                if (diff > (seq_t)1) {
                    runSum += cur;
                    prev = cur;
                } else {
                    ++runCnt;
                    if (runCnt == n) {
                        free(sqf);
                        return runSum;
                    }
                    /* start a new run with current number */
                    runSum = cur;
                    prev = cur;
                }
            }
        }
        /* Account for the final run after the loop */
        if (havePrev) {
            ++runCnt;
            if (runCnt == n) {
                free(sqf);
                return runSum;
            }
        }
        free(sqf);

        /* Increase the bound and try again */
        bound *= 2;
    }
}
