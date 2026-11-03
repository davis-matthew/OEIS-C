/* jacobi: the Jacobi symbol (a/n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_JACOBI_H
#define OEISC_UTILS_JACOBI_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: Jacobi symbol (a|n),  n odd, gcd(a,n)=1.                       */
static int jacobi(uint64_t a, uint64_t n)
{
    int result = 1;
    a %= n;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                result = -result;
        }
        /* swap a and n */
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

#endif
