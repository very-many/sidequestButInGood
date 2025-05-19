# Utility class to manage sqlite db

## responsibility:
- check if db file exits at provided path
- if not create file and populate with schema

## open questions:
- what if there are multiple db
- how to map tables/schema to db

- not filepath is provided (e.g. :memory:), the schema has to be provided somehow