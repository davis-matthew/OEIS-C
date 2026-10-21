/*
 * A271993  Main diagonal of the p-block in the periodic table of elements.
 * https://oeis.org/A271993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = atomic number of the element whose period is n and whose
 *            group is n+11 (the main diagonal of the p‑block). For n<2 or n>7
 *            the element does not exist, so the function returns 0
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The periodic table has a main diagonal p‑block element only for
       periods 2 through 7. The mapping is:
          period 2 -> group 13 -> Boron (5)
          period 3 -> group 14 -> Silicon (14)
          period 4 -> group 15 -> Arsenic (33)
          period 5 -> group 16 -> Tellurium (52)
          period 6 -> group 17 -> Astatine (85)
          period 7 -> group 18 -> Oganesson (118)
       No further main‑diagonal elements are known. */
    switch (n) {
        case 2: return 5;
        case 3: return 14;
        case 4: return 33;
        case 5: return 52;
        case 6: return 85;
        case 7: return 118;
        default: return 0;      /* undefined for n < 2 or n > 7 */
    }
}
