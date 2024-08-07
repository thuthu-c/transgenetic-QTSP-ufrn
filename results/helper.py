def format_number_abnt(num):
    if isinstance(num, str) and num.isnumeric():
        num = int(num)
    elif isinstance(num, str):
        try:
            num = float(num)
        except ValueError:
            return num  # Return as is if not a number

    # Format integer and float numbers
    if isinstance(num, int):
        return f'{num:,}'.replace(',', 'X').replace('.', ',').replace('X', '.')
    elif isinstance(num, float):
        return f'{num:,.2f}'.replace(',', 'X').replace('.', ',').replace('X', '.')
    else:
        return num  # Return as is if not a number


def unformat_abnt(num) -> float:
    if not isinstance(num, str):
        return float(num)
    
    return float(num.replace('.', '').replace(',', '.'))
